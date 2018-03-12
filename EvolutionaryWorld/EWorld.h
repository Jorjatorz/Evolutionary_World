#pragma once

#include <vector>

#include "Matrix4.h"

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

	Matrix4 VP; // Projection * View matrix
};

