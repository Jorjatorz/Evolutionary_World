#pragma once

#include "EWorld.h"

#include <vector>

#include "Matrix4.h"
#include "RandomGenerator.h"

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
	Timer* generation_timer;

	std::vector<OFood*> food_vector;
	void spawnFood();
};

