#include "EIndividual_chaser.h"

#include "RandomGenerator.h"
#include "FLog.h"
#include "Renderer.h"

EIndividual_chaser::EIndividual_chaser()
	:EIndividual()
{
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
}

EIndividual_chaser::EIndividual_chaser(int in, int out)
	:EIndividual(),
	nn(1, 1)
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
	Vector3 target = Vector3(Renderer::getInstance()->mouseX, Renderer::getInstance()->mouseY, 0.0);

	// Compute angle between individual and objective
	Vector3 dir = transform.getRotationQuaternion() * Vector3(0.0, 1.0, 0.0);
	dir.normalize();
	Vector3 objective = target - transform.getPosition();
	objective.normalize();
	float angle = Vector3::dot(objective, dir);
	if (angle > 1.0f)
		angle = 1.0f;
	if (angle < -1.0f)
		angle = 1.0f;
	angle = std::acosf(angle) * 57.295779513082320876798154814105;

	// Set angle sign depending on the side of the objective (to the left or right of the individual)
	Vector3 up = Vector3(0.0, 0.0, 1.0);
	Vector3 right = Vector3::cross(up, dir);
	if (Vector3::dot(right, objective) > 0.0)
		angle = -angle;
	angle = angle / 180.0f; // Set value from -1 to 1

	nn.execute(std::initializer_list<float>{angle});

	std::vector<float> output = nn.getOutput(NEATNN::Activation_function::TANH);
	transform.rotate(Quaternion(output.at(0) * 180.0f, Vector3(0.0, 0.0, 1.0)));

	fitness += 1 - std::abs(angle);
	transform.translate(transform.getRotationQuaternion() * Vector3(0.0, 2.0, 0.0));
	
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

void EIndividual_chaser::reset_status()
{
	transform = FTransform();
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
	transform.setRotation(Quaternion());
	fitness = 0.0;
}
