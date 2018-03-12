#include "EIndividual.h"

EIndividual::EIndividual()
	:position(0.0, 0.0, 0.0),
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

float EIndividual::getFitness() const
{
	return fitness;
}