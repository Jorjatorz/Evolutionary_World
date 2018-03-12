#include "EIndividual_orientation.h"

#include "RandomGenerator.h"

EIndividual_orientation::EIndividual_orientation()
{
	position = Vector3(RandomGenerator::randomInteger(0, 1080), RandomGenerator::randomInteger(0, 720), 0.0);
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
	fitness = 360 - std::abs(180 - orientation);

	if (fitness == 360)
		color = Vector3(0.0, 0.0, 1.0);
	else
		color = Vector3(1.0, 1.0, 0.0);
}

EIndividual* EIndividual_orientation::crossOver(EIndividual * other)
{
	EIndividual_orientation* child = new EIndividual_orientation();

	int min = orientation <= static_cast<EIndividual_orientation*>(other)->orientation ? orientation : static_cast<EIndividual_orientation*>(other)->orientation;
	int max = orientation > static_cast<EIndividual_orientation*>(other)->orientation ? orientation : static_cast<EIndividual_orientation*>(other)->orientation;
	if(min == max)
		child->orientation = min;
	else
		child->orientation = RandomGenerator::randomInteger(min, max);

	return child;
}

void EIndividual_orientation::mutate()
{
	if (RandomGenerator::randomFloat() < 0.06)
	{
		orientation = RandomGenerator::randomInteger(0, 359);
	}
}

EIndividual * EIndividual_orientation::clone() const
{
	return new EIndividual_orientation(*this);
}
