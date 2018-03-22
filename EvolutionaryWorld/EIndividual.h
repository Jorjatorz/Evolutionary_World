#pragma once

#include "Object.h"

class EIndividual : public Object
{
public:
	EIndividual();
	EIndividual(const EIndividual& other);
	virtual ~EIndividual();

	float getFitness() const;

	// Resets the individual status (not genome)
	virtual void reset_status();

	virtual void evaluate() = 0;
	virtual EIndividual* crossOver(EIndividual* other) = 0; // Returns a new allocated EIndividual (user should delete this new EIndividual manually)
	virtual void mutate() = 0;

	virtual EIndividual* clone() const = 0;

protected:
	float fitness;
};

