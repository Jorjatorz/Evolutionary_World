#pragma once
#include "EIndividual.h"

#include "NEATNN.h"

class EIndividual_chaser :	public EIndividual
{
public:
	EIndividual_chaser();
	EIndividual_chaser(int in, int out);
	EIndividual_chaser(const EIndividual_chaser& copy);
	~EIndividual_chaser();

	// Heredado vía EIndividual
	virtual void evaluate() override;
	virtual EIndividual * crossOver(EIndividual * other) override;
	virtual void mutate() override;
	virtual EIndividual * clone() const override;

private:
	NEATNN nn;
};

