#pragma once

#include "EIndividualRenderer.h"
#include "FTransform.h"

class EIndividual
{
public:
	EIndividual();
	EIndividual(const EIndividual& other);
	virtual ~EIndividual();

	void render(Matrix4 mat);

	float getFitness() const;

	// Resets the individual status (not genome)
	virtual void reset_status();

	virtual void evaluate() = 0;
	virtual EIndividual* crossOver(EIndividual* other) = 0; // Returns a new allocated EIndividual (user should delete this new EIndividual manually)
	virtual void mutate() = 0;

	virtual EIndividual* clone() const = 0;

	FTransform* getTransform_pointer();

protected:
	float fitness;

	EIndividualRenderer renderer;
	Vector3 color;

	FTransform transform;
};

