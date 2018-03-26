#pragma once
#include "EIndividual.h"

#include "NEATNN.h"

class FoodDetectorComponent;

class EIndividual_eater : public EIndividual
{
public:
	EIndividual_eater();
	EIndividual_eater(int in, int out);
	EIndividual_eater(const EIndividual_eater& copy);
	~EIndividual_eater();

	bool isDead(); // True if the health is < 0.0

	// Heredado vía EIndividual
	virtual void evaluate() override;
	virtual EIndividual * crossOver(EIndividual * other) override;
	virtual void mutate() override;
	virtual EIndividual * clone() const override;
	virtual void reset_status() override;

private:
	NEATNN nn;
	float health; // From 0 to 1.0
	FoodDetectorComponent* food_detector;
};

