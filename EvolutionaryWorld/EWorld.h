#pragma once

#include <vector>

#include "EIndividual.h"
#include "Matrix4.h"

class EWorld
{
public:
	EWorld();
	~EWorld();

	void renderPopulation();

	void processPopulation();

private:
	std::vector<EIndividual> population;

	void evaluate();
	void selection();
	void crossOver_and_mutation();

	Matrix4 VP; // Projection * View matrix
};

