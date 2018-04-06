#pragma once

#include "EWorld.h"

#include <vector>

#include "Matrix4.h"
#include "RandomGenerator.h"
#include "UIWidget.h"

class EIndividual;
class Timer;
class OFood;

// World that spawns food
class EWorld_food : public EWorld
{
public:
	EWorld_food();
	~EWorld_food();

	virtual void tickWorld();

	std::vector<OFood*>* getFoodsVector_ptr();

private:
	std::vector<EIndividual*> population;

	void evaluate();
	EIndividual* selectIndividual();
	void crossOver_and_mutation();

	RandomGenerator rand_generator;

	// Timer to control food spawning
	Timer* food_spawner_timer;

	std::vector<OFood*> food_vector;
	void spawnFood();

	// Private class for configuration
	class ConfigWidget : public UIWidget
	{
	public:
		ConfigWidget(EWorld_food* w) : UIWidget("EWorldFood_Config"), world(w) {}
		~ConfigWidget() {}

		// Heredado vía UIWidget
		virtual void render() override;

	private:
		EWorld_food* world;
	} configWidget;

	// Simulation information
	float globalBestFitness;
	float currentBestFitness;
};

