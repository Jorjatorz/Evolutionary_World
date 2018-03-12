#pragma once
#include "EIndividual.h"
class EIndividual_color : public EIndividual
{
public:
	EIndividual_color();
	EIndividual_color(const EIndividual_color& copy);
	~EIndividual_color();

	// Heredado vía EIndividual
	virtual void evaluate() override;
	virtual EIndividual* crossOver(EIndividual* other) override;
	virtual void mutate() override;

	// Heredado vía EIndividual
	virtual EIndividual_color* clone() const override;
};

