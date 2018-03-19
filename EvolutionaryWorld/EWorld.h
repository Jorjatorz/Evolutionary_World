#pragma once

#include <vector>

#include "Matrix4.h"
#include "RandomGenerator.h"

class EIndividual;

class EWorld
{
public:
	EWorld();
	~EWorld();

	void renderPopulation();

	void processPopulation();

private:
	std::vector<EIndividual*> population;

	void evaluate();
	void selection();
	void crossOver_and_mutation();

	RandomGenerator rand_generator;

	Matrix4 VP; // Projection * View matrix
};

