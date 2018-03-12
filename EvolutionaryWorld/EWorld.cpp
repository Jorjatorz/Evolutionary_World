#include "EWorld.h"

#include "RandomGenerator.h"


EWorld::EWorld()
{
	VP = Matrix4::createOrthoMatrix(1080, 720, 0.0, 1000);
	VP.translate(Vector3(-1080/2.0, -720/2.0, 0.0)); // Put the camera centered so the bottom left corner is (0.0, 0.0)

	for (int i = 0; i < 10; i++)
	{
		population.emplace_back();
	}
}


EWorld::~EWorld()
{
}

void EWorld::renderPopulation()
{
	for (auto& individual : population)
	{
		individual.render(VP);
	}
}

void EWorld::processPopulation()
{
	evaluate();
	selection();
	crossOver_and_mutation();
}

void EWorld::evaluate()
{
	for (auto& ind : population)
	{
		ind.evaluate();
	}
}

void EWorld::selection()
{
	// Tournament selection
	std::vector<EIndividual> selected;
	for (int i = 0; i < population.size(); i++)
	{
		EIndividual a = population.at(RandomGenerator::randomInteger(0, population.size()));
		EIndividual b = population.at(RandomGenerator::randomInteger(0, population.size()));
		EIndividual c = population.at(RandomGenerator::randomInteger(0, population.size()));

		if (a.getFitness() > b.getFitness())
		{
			a.getFitness() > c.getFitness() ? selected.push_back(a) : selected.push_back(c);
		}
		else
		{
			c.getFitness() > b.getFitness() ? selected.push_back(c) : selected.push_back(b);
		}
	}

	population = std::move(selected);
}

void EWorld::crossOver_and_mutation()
{
	for (int i = 0; i < population.size(); i++)
	{
		if (RandomGenerator::randomFloat() < 0.6)
		{
			population[i] = population.at(i).crossOver(population.at(RandomGenerator::randomInteger(0, population.size())));

			// Mutate
			population[i].mutate();
		}
	}
}
