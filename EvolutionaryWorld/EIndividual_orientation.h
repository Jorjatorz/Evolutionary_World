#pragma once
#include "EIndividual.h"
class EIndividual_orientation :	public EIndividual
{
public:
	EIndividual_orientation();
	EIndividual_orientation(const EIndividual_orientation& other);
	~EIndividual_orientation();

	// Heredado vía EIndividual
	virtual void evaluate() override;
	virtual EIndividual * crossOver(EIndividual * other) override;
	virtual void mutate() override;
	virtual EIndividual * clone() const override;
};

