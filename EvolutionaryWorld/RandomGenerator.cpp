#include "RandomGenerator.h"

#include <cstdlib>

RandomGenerator::RandomGenerator()
{
}


RandomGenerator::~RandomGenerator()
{
}

int RandomGenerator::randomInteger(int min, int max)
{
	return std::rand() % (max - min) + min;
}

float RandomGenerator::randomFloat()
{
	return std::rand() / (float)RAND_MAX;
}
