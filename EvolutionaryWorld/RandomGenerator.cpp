#include "RandomGenerator.h"

#include <ctime>

#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

boost::random::mt19937 RandomGenerator::generator(std::time(0));

RandomGenerator::RandomGenerator()
{
}


RandomGenerator::~RandomGenerator()
{
}

int RandomGenerator::randomInteger(int min, int max)
{
	boost::random::uniform_int_distribution<> dist(min, max);
	return dist(generator);
}

float RandomGenerator::randomFloat(float min, float max)
{
	boost::random::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}
