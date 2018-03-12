#pragma once

#include "EIndividualRenderer.h"
#include "Vector3.h"

class EIndividual
{
public:
	EIndividual();
	EIndividual(const EIndividual& other);
	virtual ~EIndividual();

	void render(Matrix4 transform);

	float getFitness() const;

	virtual void evaluate() = 0;
	virtual EIndividual* crossOver(EIndividual* other) = 0; // Returns a new allocated EIndividual (user should delete this new EIndividual manually)
	virtual void mutate() = 0;

	virtual EIndividual* clone() const = 0;

protected:
	float fitness;

	EIndividualRenderer renderer;

	Vector3 position;
	float orientation; // Angle between the head and the top border of the screen

	Vector3 color;
};

