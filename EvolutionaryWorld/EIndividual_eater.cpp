#include "EIndividual_eater.h"



#include "RandomGenerator.h"
#include "FLog.h"
#include "Renderer.h"
#include "FoodDetectorComponent.h"
#include "OFood.h"
#include "EIndividualRendererComponent.h"
#include "TimerManager.h"

EIndividual_eater::EIndividual_eater()
	:EIndividual_eater(0, 0)
{
}

EIndividual_eater::EIndividual_eater(int in, int out)
	:EIndividual(),
	nn(in, out),
	health(1.0f)
{
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));

	this->getComponentByClass<EIndividualRendererComponent>()->setColor(Vector3(1.0f - health, health, 0.0));

	food_detector = this->addComponent<FoodDetectorComponent>();
}

EIndividual_eater::EIndividual_eater(const EIndividual_eater & copy)
	:EIndividual(copy),
	health(copy.health)
{
	nn = copy.nn;
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));

	this->getComponentByClass<EIndividualRendererComponent>()->setColor(Vector3(1.0f - health, health, 0.0));
	food_detector = this->getComponentByClass<FoodDetectorComponent>();
}


EIndividual_eater::~EIndividual_eater()
{
}

bool EIndividual_eater::isDead()
{
	return health <= 0.0f;
}

void EIndividual_eater::evaluate()
{
	bool eaten;
	OFood* food = food_detector->getClosestFood(eaten);
	float velocity = 0.0f;
	if (food != nullptr)
	{
		Vector3 target = Vector3(food->getTransform_pointer()->getPositionPtr()->x, food->getTransform_pointer()->getPositionPtr()->y, 0.0);

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
		transform.rotate(Quaternion(output.at(0) * 180.0f * 3 * TimerManager::getInstance()->getWorldDeltaSeconds(), Vector3(0.0, 0.0, 1.0)));
		velocity = output.at(1);

		//fitness += 1 - std::abs(angle);
	}
	else if (eaten)
	{
		velocity = 0.25;
		health += 0.5;
		health = health > 1.0f ? health = 1.0 : health;
	}

	velocity = velocity * 280 * TimerManager::getInstance()->getWorldDeltaSeconds(); // Frame independent
	transform.translate(transform.getRotationQuaternion() * Vector3(0.0, velocity, 0.0));
	health -= 0.125 * TimerManager::getInstance()->getWorldDeltaSeconds();
	this->getComponentByClass<EIndividualRendererComponent>()->setColor(Vector3(1.0f - health, health, 0.0));

	fitness++;
}

EIndividual * EIndividual_eater::crossOver(EIndividual * other)
{
	EIndividual_eater* child = new EIndividual_eater();

	// Compare fitness, parent1 is the best from both parents
	if (fitness >= static_cast<EIndividual_eater*>(other)->fitness)
		child->nn = nn.crossOver(static_cast<EIndividual_eater*>(other)->nn);
	else
		child->nn = static_cast<EIndividual_eater*>(other)->nn.crossOver(nn);

	child->getTransform_pointer()->setPosition(transform.getPosition());
	child->getTransform_pointer()->setRotation(transform.getRotationQuaternion());

	return child;
}

void EIndividual_eater::mutate()
{
	nn.mutate();
}

EIndividual * EIndividual_eater::clone() const
{
	return new EIndividual_eater(*this);
}

void EIndividual_eater::reset_status()
{
	transform = FTransform();
	RandomGenerator generator;
	transform.setPosition(Vector3(generator.randomInteger(0, 1080), generator.randomInteger(0, 720), 0.0));
	transform.setRotation(Quaternion());
	fitness = 0.0;
}
