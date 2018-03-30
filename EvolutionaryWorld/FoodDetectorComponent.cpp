#include "FoodDetectorComponent.h"

#include "EWorld_food.h"
#include "OFood.h"

FoodDetectorComponent::FoodDetectorComponent(Object * owner)
	:Component(owner)
{
}

FoodDetectorComponent::~FoodDetectorComponent()
{
}

OFood* FoodDetectorComponent::getClosestFood(bool& eaten, OFood& foodEaten)
{
	OFood* closest = nullptr;
	eaten = false;

	EWorld_food* world = dynamic_cast<EWorld_food*>(EWorld::getInstance());
	if (world != nullptr)
	{
		std::vector<OFood*>* foods = world->getFoodsVector_ptr();
		auto minDistance_it = foods->begin();
		float minDistance = INT32_MAX;
		for (auto it = foods->begin(); it != foods->end(); ++it)
		{
			auto food = *it;
			float dist = Vector3::length(*(food->getTransform_pointer()->getPositionPtr()) - *(owner->getTransform_pointer()->getPositionPtr()));
			if (dist < minDistance)
			{
				closest = food;
				minDistance = dist;
				minDistance_it = it;
			}
		}


		// If the distance is small, eat it
		if (minDistance < 10.0f)
		{
			foodEaten = *closest;

			delete closest;
			closest = nullptr;
			foods->erase(minDistance_it);

			eaten = true;
		}
	}

	return closest;
}

Component * FoodDetectorComponent::clone(Object * newOwner) const
{
	return new FoodDetectorComponent(newOwner);
}
