#include "EIndividual.h"
#include "EIndividualRendererComponent.h"

EIndividual::EIndividual()
	:fitness(0.0)
{
	this->addComponent<EIndividualRendererComponent>();
}

EIndividual::EIndividual(const EIndividual& other)
	:Object(other)
{
	fitness = other.fitness;
}


EIndividual::~EIndividual()
{
}

float EIndividual::getFitness() const
{
	return fitness;
}

void EIndividual::reset_status()
{
	transform = FTransform();
	fitness = 0.0;
}
