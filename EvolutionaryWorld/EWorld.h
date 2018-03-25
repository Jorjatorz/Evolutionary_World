#pragma once

// Interface class that defines a world
class EWorld
{
public:
	EWorld();
	virtual ~EWorld();

	/*void processPopulation_classic();
	void processPopulation_steps();*/
	virtual void tickWorld() = 0;
};

