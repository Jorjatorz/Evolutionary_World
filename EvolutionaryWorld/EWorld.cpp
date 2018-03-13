#include "EWorld.h"

#include "RandomGenerator.h"

#include "EIndividual_orientation.h"

EWorld::EWorld()
{
	VP = Matrix4::createOrthoMatrix(1080, 720, 0.0, 1000);
	VP.translate(Vector3(-1080/2.0, -720/2.0, 0.0)); // Put the camera centered so the bottom left corner is (0.0, 0.0)

	for (int i = 0; i < 10; i++)
	{
		population.emplace_back(new EIndividual_orientation());
	}
	evaluate();
}


EWorld::~EWorld()
{
	for (auto& individual : population)
	{
		delete individual;
	}
}

void EWorld::renderPopulation()
{
	for (auto& individual : population)
	{
		individual->render(VP);
	}
}

void EWorld::processPopulation()
{
	selection();
	crossOver_and_mutation();
	evaluate();
}

void EWorld::evaluate()
{
	for (auto& ind : population)
	{
		ind->evaluate();
	}
}

void EWorld::selection()
{
	// Tournament selection
	std::vector<EIndividual*> selected;
	for (int i = 0; i < population.size(); i++)
	{
		EIndividual* a = population.at(RandomGenerator::randomInteger(0, population.size()));
		EIndividual* b = population.at(RandomGenerator::randomInteger(0, population.size()));
		EIndividual* c = population.at(RandomGenerator::randomInteger(0, population.size()));

		if (a->getFitness() > b->getFitness())
		{
			a->getFitness() > c->getFitness() ? selected.emplace_back(a->clone()) : selected.emplace_back(c->clone());
		}
		else
		{
			c->getFitness() > b->getFitness() ? selected.emplace_back(c->clone()) : selected.emplace_back(b->clone());
		}
	}

	for (auto& individual : population)
	{
		delete individual;
	}
	population = std::move(selected);
}

void EWorld::crossOver_and_mutation()
{
	for (int i = 0; i < population.size(); i++)
	{
		if (RandomGenerator::randomFloat() < 0.6)
		{
			auto child = population.at(i)->crossOver(population.at(RandomGenerator::randomInteger(0, population.size())));
			delete population[i];
			population[i] = child;

			// Mutate
			population[i]->mutate();
		}
	}
}
