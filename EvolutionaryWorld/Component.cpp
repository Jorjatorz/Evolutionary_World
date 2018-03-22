#include "Component.h"



Component::Component(Object* owner)
	:owner(owner)
{
}


Component::~Component()
{
}

Component* Component::clone(Object* newOwner) const
{
	return new Component(newOwner);
}
