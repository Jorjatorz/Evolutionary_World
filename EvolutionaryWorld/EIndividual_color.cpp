#include "EIndividual_color.h"

#include "Matrix4.h"
#include "RandomGenerator.h"

EIndividual_color::EIndividual_color()
{
	position = Vector3(RandomGenerator::randomInteger(0, 1080), RandomGenerator::randomInteger(0, 720), 0.0);
}

EIndividual_color::EIndividual_color(const EIndividual_color & copy)
	:EIndividual(copy)
{
}


EIndividual_color::~EIndividual_color()
{
}

void EIndividual_color::evaluate()
{
	fitness = 3 - Vector3::length(Vector3(1.0, 1.0, 1.0) - color);
}

EIndividual* EIndividual_color::crossOver(EIndividual * other)
{
	EIndividual_color* child = new EIndividual_color(*this);
	for (int i = RandomGenerator::randomInteger(0, 3); i < 3; i++)
	{
		switch (i)
		{
		case 0:
			child->color.x = static_cast<EIndividual_color*>(other)->color.x;
			break;
		case 1:
			child->color.y = static_cast<EIndividual_color*>(other)->color.y;
			break;
		case 2:
			child->color.z = static_cast<EIndividual_color*>(other)->color.z;
			break;
		}
	}

	child->position = Vector3(RandomGenerator::randomInteger(0, 1080), RandomGenerator::randomInteger(0, 720), 0.0);

	return child;
}

void EIndividual_color::mutate()
{
	for (int i = 0; i < 3; i++)
	{
		if (RandomGenerator::randomFloat() < 0.06)
		{
			switch (i)
			{
			case 0:
				color.x = RandomGenerator::randomFloat();
				break;
			case 1:
				color.y = RandomGenerator::randomFloat();
				break;
			case 2:
				color.z = RandomGenerator::randomFloat();
				break;
			}
		}
	}
}

EIndividual_color* EIndividual_color::clone() const
{
	return new EIndividual_color(*this);
}
