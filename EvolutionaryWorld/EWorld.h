#pragma once

#include "Singleton.h"

// Interface class that defines a world
class EWorld : public Singleton<EWorld>
{
public:
	EWorld();
	virtual ~EWorld();

	/*void processPopulation_classic();
	void processPopulation_steps();*/
	virtual void tickWorld() = 0;
};

