#pragma once

#include "EWorld.h"

#include <vector>

#include "Matrix4.h"
#include "RandomGenerator.h"

class EIndividual;
class Timer;

// World that spawns food
class EWorld_food : public EWorld
{
public:
	EWorld_food();
	~EWorld_food();

	virtual void tickWorld();

private:
	std::vector<EIndividual*> population;

	void evaluate();
	void selection();
	void crossOver_and_mutation();

	RandomGenerator rand_generator;
	Timer* generation_timer;
};

