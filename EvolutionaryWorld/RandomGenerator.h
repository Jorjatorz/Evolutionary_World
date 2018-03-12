#pragma once
class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	// Generates a random integer between [min, max)
	static int randomInteger(int min, int max);

	static float randomFloat();
};

