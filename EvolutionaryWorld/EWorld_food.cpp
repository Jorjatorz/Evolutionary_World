#include "EWorld_food.h"

#include "RandomGenerator.h"
#include "TimerManager.h"
#include "EIndividual_chaser.h"

EWorld_food::EWorld_food()
	:EWorld()
{
	for (int i = 0; i < 50; i++)
	{
		population.emplace_back(new EIndividual_chaser(3, 3));
	}
	evaluate();

	generation_timer = TimerManager::getInstance()->addTimer();
}


EWorld_food::~EWorld_food()
{
	for (auto& individual : population)
	{
		delete individual;
	}
}

void EWorld_food::tickWorld()
{
	if (generation_timer->getTime_milliseconds() > 3000)
	{
		selection();
		crossOver_and_mutation();

		// Set random position
		for (auto& ind : population)
		{

			ind->reset_status();
		}

		generation_timer->reset();
	}
	evaluate();
}

void EWorld_food::evaluate()
{
	for (auto& ind : population)
	{
		ind->evaluate();
	}
}

void EWorld_food::selection()
{
	// Tournament selection
	std::vector<EIndividual*> selected;
	for (int i = 0; i < population.size(); i++)
	{
		EIndividual* a = population.at(rand_generator.randomInteger(0, population.size() - 1));
		EIndividual* b = population.at(rand_generator.randomInteger(0, population.size() - 1));
		EIndividual* c = population.at(rand_generator.randomInteger(0, population.size() - 1));

		// Maximizacion
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

void EWorld_food::crossOver_and_mutation()
{
	for (int i = 0; i < population.size(); i++)
	{
		if (rand_generator.randomFloat() < 0.6)
		{
			auto child = population.at(i)->crossOver(population.at(rand_generator.randomInteger(0, population.size() - 1)));
			delete population[i];
			population[i] = child;

			// Mutate
			population[i]->mutate();
		}
	}
}
