#pragma once

#include <boost/random/mersenne_twister.hpp>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	// Generates a random integer between [min, max]
	int randomInteger(int min, int max);

	// Generates a random integer between [min, max)
	float randomFloat(float min = 0.0, float max = 1.0);

private:
	static boost::mt19937 generator;
};

