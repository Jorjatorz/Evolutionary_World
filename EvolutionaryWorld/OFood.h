#pragma once
#include "Object.h"

// Game Object that represents a food
class OFood : public Object
{
public:
	OFood();
	~OFood();

	int type; // 0 good, 1 poison
};

