#include "EWorld_food.h"

#include "RandomGenerator.h"
#include "TimerManager.h"
#include "EIndividual_eater.h"
#include "OFood.h"

EWorld_food::EWorld_food()
	:EWorld(),
	globalBestFitness(0.0f),
	currentBestFitness(0.0f),
	configWidget(this)
{
	for (int i = 0; i < 50; i++)
	{
		population.emplace_back(new EIndividual_eater(2, 2));
	}
	evaluate();

	food_spawner_timer = TimerManager::getInstance()->addTimer(this, &EWorld_food::spawnFood, 100.0f, true);
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
	currentBestFitness = 0.0f;
	for (auto ind_it = population.begin(); ind_it != population.end();)
	{
		(*ind_it)->evaluate();

		float fitness = (*ind_it)->getFitness();
		if (fitness > currentBestFitness)
		{
			currentBestFitness = fitness;
			if (currentBestFitness > globalBestFitness)
				globalBestFitness = currentBestFitness;
		}

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
	// K-Tournament selection
	int k = rand_generator.randomInteger(2, std::min(10u, population.size() - 1)); // Set randomly the number of individuals to compare
	EIndividual* selected = population.at(rand_generator.randomInteger(0, population.size() - 1));
	float maxFitness = selected->getFitness();
	for (int i = 1; i < k; i++)
	{
		EIndividual* ind = population.at(rand_generator.randomInteger(0, population.size() - 1));
		if (ind->getFitness() > maxFitness)
		{
			selected = ind;
			maxFitness = ind->getFitness();
		}
	}

	return selected;
	// Tournament selection
	//EIndividual* a = population.at(rand_generator.randomInteger(0, population.size() - 1));
	//EIndividual* b = population.at(rand_generator.randomInteger(0, population.size() - 1));
	//EIndividual* c = population.at(rand_generator.randomInteger(0, population.size() - 1));

	//// Maximizacion
	//if (a->getFitness() > b->getFitness())
	//{
	//	if (a->getFitness() > c->getFitness())
	//	{
	//		return a;
	//	}
	//	else
	//	{
	//		return c;
	//	}
	//}
	//else
	//{
	//	if (c->getFitness() > b->getFitness())
	//	{
	//		return c;
	//	}
	//	else
	//	{
	//		return b;
	//	}
	//}
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

#include "IMGUI\imgui.h"
void EWorld_food::ConfigWidget::render()
{
	int speed = TimerManager::getInstance()->getWorldSpeed();
	int spawn_time = world->food_spawner_timer->getTimerDuration();
	float weight_mut = NEATNN::weight_mutation_probability;
	float add_connection_prob = NEATNN::add_connection_probability;
	float remove_connection_prob = NEATNN::remove_connection_probability;
	float add_node_prob = NEATNN::add_node_probability;

	ImGui::Begin("World config");
	ImGui::SliderInt("Speed", &speed, 0, 10);
	ImGui::InputInt("Spawn time", &spawn_time);
	ImGui::Separator();
	ImGui::SliderFloat("Weight mutation", &weight_mut, 0.0, 1.0, "%.2f");
	ImGui::SliderFloat("Add connection", &add_connection_prob, 0.0, 1.0, "%.2f");
	ImGui::SliderFloat("Remove connection", &remove_connection_prob, 0.0, 1.0, "%.2f");
	ImGui::SliderFloat("Add node", &add_node_prob, 0.0, 1.0, "%.2f");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "Global best fitness: %f", world->globalBestFitness);
	ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "Current best fitness: %f", world->currentBestFitness);
	ImGui::End();

	TimerManager::getInstance()->setWorldSpeed(speed);
	world->food_spawner_timer->setTimerDuration(spawn_time);
	NEATNN::weight_mutation_probability = weight_mut;
	NEATNN::add_connection_probability = add_connection_prob;
	NEATNN::remove_connection_probability = remove_connection_prob;
	NEATNN::add_node_probability = add_node_prob;
}
