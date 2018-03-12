#include "EIndividual.h"

#include "Matrix4.h"
#include "RandomGenerator.h"

EIndividual::EIndividual()
	:position(RandomGenerator::randomInteger(0, 1080), RandomGenerator::randomInteger(0, 720), 0.0),
	orientation(0.0),
	color(1.0, 1.0, 0.0),
	fitness(0.0)
{
}

EIndividual::EIndividual(const EIndividual& other)
{
	position = other.position;
	orientation = other.orientation;
	color = other.color;
	fitness = other.fitness;
}


EIndividual::~EIndividual()
{
}

void EIndividual::render(Matrix4 transform)
{
	transform.translate(position);
	transform.rotate(-orientation, Vector3(0.0, 0.0, 1.0)); // Negate the orientation so its clockwise
	renderer.render(transform, color);
}

float EIndividual::getFitness()
{
	return fitness;
}

void EIndividual::evaluate()
{
	fitness = 3 - Vector3::length(Vector3(1.0, 1.0, 1.0) - color);
}

EIndividual EIndividual::crossOver(EIndividual other)
{
	EIndividual toReturn(*this);
	for (int i = RandomGenerator::randomInteger(0, 3); i < 3; i++)
	{
		switch (i)
		{
		case 0:
			toReturn.color.x = other.color.x;
			break;
		case 1:
			toReturn.color.y = other.color.y;
			break;
		case 2:
			toReturn.color.z = other.color.z;
			break;
		}
	}

	toReturn.position = Vector3(RandomGenerator::randomInteger(0, 1080), RandomGenerator::randomInteger(0, 720), 0.0);

	return toReturn;
}

void EIndividual::mutate()
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
