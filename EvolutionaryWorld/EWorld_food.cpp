#include "EWorld_food.h"

#include "RandomGenerator.h"
#include "TimerManager.h"
#include "EIndividual_eater.h"
#include "OFood.h"

EWorld_food::EWorld_food()
	:EWorld()
{
	for (int i = 0; i < 50; i++)
	{
		population.emplace_back(new EIndividual_eater(1, 2));
	}
	evaluate();

	generation_timer = TimerManager::getInstance()->addTimer();

	// Add spawner
	TimerManager::getInstance()->addTimer(this, &EWorld_food::spawnFood, 100, true);
}


EWorld_food::~EWorld_food()
{
	for (auto& food : food_vector)
	{
		delete food;
	}
	for (auto& individual : population)
	{
		delete individual;
	}
}

void EWorld_food::tickWorld()
{
	evaluate();
}

std::vector<OFood*>* EWorld_food::getFoodsVector_ptr()
{
	return &food_vector;
}

void EWorld_food::evaluate()
{
	for (auto ind_it = population.begin(); ind_it != population.end();)
	{
		(*ind_it)->evaluate();

		if (static_cast<EIndividual_eater*>(*ind_it)->isDead())
		{
			// Select random element
			EIndividual* parent2 = selectIndividual();
			// Cross
			auto child = (*ind_it)->crossOver(parent2);
			delete *ind_it;
			ind_it = population.erase(ind_it);
			population.push_back(child);

			// Mutate
			child->mutate();
		}
		else
		{
			++ind_it;
		}
	}
}

EIndividual* EWorld_food::selectIndividual()
{
	// Tournament selection
	EIndividual* a = population.at(rand_generator.randomInteger(0, population.size() - 1));
	EIndividual* b = population.at(rand_generator.randomInteger(0, population.size() - 1));
	EIndividual* c = population.at(rand_generator.randomInteger(0, population.size() - 1));

	// Maximizacion
	if (a->getFitness() > b->getFitness())
	{
		if (a->getFitness() > c->getFitness())
		{
			return a;
		}
		else
		{
			return c;
		}
	}
	else
	{
		if (c->getFitness() > b->getFitness())
		{
			return c;
		}
		else
		{
			return b;
		}
	}
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

void EWorld_food::spawnFood()
{
	food_vector.push_back(new OFood());
}
