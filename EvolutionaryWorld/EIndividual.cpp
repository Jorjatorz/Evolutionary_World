#include "EIndividual.h"

EIndividual::EIndividual()
	:color(1.0, 1.0, 0.0),
	fitness(0.0)
{
}

EIndividual::EIndividual(const EIndividual& other)
{
	transform = other.transform;
	color = other.color;
	fitness = other.fitness;
}


EIndividual::~EIndividual()
{
}

void EIndividual::render(Matrix4 mat)
{
	mat = mat * transform.getTransformationMatrix();
	renderer.render(mat, color);
}

float EIndividual::getFitness() const
{
	return fitness;
}

void EIndividual::reset_status()
{
	transform = FTransform();
	transform.setPosition(Vector3(540, 360, 0.0));
	color = Vector3(1.0);
	fitness = 0.0;
}

FTransform * EIndividual::getTransform_pointer()
{
	return &transform;
}
