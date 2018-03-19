#include "EIndividual_chaser.h"

#include "RandomGenerator.h"

EIndividual_chaser::EIndividual_chaser()
{
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
}

EIndividual_chaser::EIndividual_chaser(int in, int out)
	:nn(3, 3)
{
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
}

EIndividual_chaser::EIndividual_chaser(const EIndividual_chaser & copy)
	:EIndividual(copy)
{
	nn = copy.nn;
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
}


EIndividual_chaser::~EIndividual_chaser()
{
}

void EIndividual_chaser::evaluate()
{
	Vector3 target = Vector3(255, 0.0, 0.0);
	nn.execute(std::initializer_list<float>{target.x, target.y, target.z});

	std::vector<float> output = nn.getOutput();
	Vector3 output_v = Vector3(output.at(0), output.at(1), output.at(2));


	Vector3 aux = target - output_v;
	fitness = 100000 - std::abs(aux.x) - std::abs(aux.y) - std::abs(aux.z);

	color = output_v / 255.0;
	
}

EIndividual * EIndividual_chaser::crossOver(EIndividual * other)
{
	EIndividual_chaser* child = new EIndividual_chaser();

	// Compare fitness, parent1 is the best from both parents
	if(fitness >= static_cast<EIndividual_chaser*>(other)->fitness)
		child->nn = nn.crossOver(static_cast<EIndividual_chaser*>(other)->nn);
	else
		child->nn = static_cast<EIndividual_chaser*>(other)->nn.crossOver(nn);

	return child;
}

void EIndividual_chaser::mutate()
{
	nn.mutate();
}

EIndividual * EIndividual_chaser::clone() const
{
	return new EIndividual_chaser(*this);
}
