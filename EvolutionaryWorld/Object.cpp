#include "Object.h"



Object::Object()
{
}

Object::Object(const Object & other)
	:transform(transform)
{
	// Copy components
	for (auto& component : other.components_list)
	{
		components_list.push_back(component->clone(this));
	}
}

void Object::operator=(const Object & other)
{
	// Clear previous components
	for (auto& component : components_list)
	{
		delete component;
	}
	components_list.clear();


	transform = other.transform;
	 // Copy components
	for (auto& component : other.components_list)
	{
		components_list.push_back(component->clone(this));
	}
}


Object::~Object()
{
	for (auto& component : components_list)
	{
		delete component;
	}
}

FTransform * Object::getTransform_pointer()
{
	return &transform;
}