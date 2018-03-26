#pragma once
#include "Component.h"

class OFood;

class FoodDetectorComponent : public Component
{
public:
	FoodDetectorComponent(Object* owner);
	~FoodDetectorComponent();

	// Returns the closest food to the owner object
	OFood* getClosestFood(bool& eaten);

	virtual Component* clone(Object* newOwner) const;
};

