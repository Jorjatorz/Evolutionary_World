#include "OFood.h"

#include "EIndividualRendererComponent.h"
#include "RandomGenerator.h"

OFood::OFood()
{
	RandomGenerator generator;

	auto* comp = this->addComponent<EIndividualRendererComponent>();
	if (generator.randomFloat() < 0.95)
	{
		type = 0;
		comp->setColor(Vector3(0.0, 0.0, 1.0));
	}
	else
	{
		type = 1;
		comp->setColor(Vector3(0.0, 1.0, 1.0));
	}
	
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
	transform.setScale(Vector3(0.5));
}


OFood::~OFood()
{
}
