#pragma once

#include "EIndividualRenderer.h"
#include "Vector3.h"

class EIndividual
{
public:
	EIndividual();
	EIndividual(const EIndividual& other);
	~EIndividual();

	void render(Matrix4 transform);

	float getFitness();

	void evaluate();
	EIndividual crossOver(EIndividual other);
	void mutate();

private:
	float fitness;

	EIndividualRenderer renderer;

	Vector3 position;
	float orientation; // Angle between the head and the top border of the screen

	Vector3 color;
};

