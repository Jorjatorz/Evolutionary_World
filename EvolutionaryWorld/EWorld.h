#pragma once

#include <vector>

#include "Matrix4.h"
#include "RandomGenerator.h"

class EIndividual;
class Timer;

class EWorld
{
public:
	EWorld();
	~EWorld();

	void renderPopulation();

	void processPopulation_classic();
	void processPopulation_steps();

private:
	std::vector<EIndividual*> population;

	void evaluate();
	void selection();
	void crossOver_and_mutation();

	RandomGenerator rand_generator;
	Timer* generation_timer;

	Matrix4 VP; // Projection * View matrix
};

