#include "EIndividual_orientation.h"

#include "RandomGenerator.h"

EIndividual_orientation::EIndividual_orientation()
{
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
}

EIndividual_orientation::EIndividual_orientation(const EIndividual_orientation & other)
	:EIndividual(other)
{
}


EIndividual_orientation::~EIndividual_orientation()
{
}

void EIndividual_orientation::evaluate()
{
	float orientation = transform.getRotationQuaternion().toEuler().z;
	fitness = 360 - std::abs(180 - orientation);

	color = Vector3(1.0 - fitness/360, 0.0, fitness / 360);
}

EIndividual* EIndividual_orientation::crossOver(EIndividual * other)
{
	EIndividual_orientation* child = new EIndividual_orientation();

	float orientation = transform.getRotationQuaternion().toEuler().z;
	float other_orientation = static_cast<EIndividual_orientation*>(other)->transform.getRotationQuaternion().toEuler().z;
	float min = orientation <= other_orientation ? orientation : other_orientation;
	float max = orientation > other_orientation ? orientation : other_orientation;
	if(min == max)
		child->transform.setRotation(Quaternion(min, Vector3(0.0, 0.0, 1.0)));
	else
	{
		RandomGenerator generator;
		child->transform.setRotation(Quaternion(generator.randomInteger(min, max), Vector3(0.0, 0.0, 1.0)));
	}

	return child;
}

void EIndividual_orientation::mutate()
{
	RandomGenerator generator;
	if (generator.randomFloat() < 0.06)
	{
		transform.setRotation(Quaternion(generator.randomInteger(0, 359), Vector3(0.0, 0.0, 1.0)));
	}
}

EIndividual * EIndividual_orientation::clone() const
{
	return new EIndividual_orientation(*this);
}
