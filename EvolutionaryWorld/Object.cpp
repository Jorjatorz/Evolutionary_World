#include "Object.h"



Object::Object()
{
}

Object::Object(const Object & other)
	:transform(transform)
{
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