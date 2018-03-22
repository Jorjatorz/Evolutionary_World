#pragma once


class Object;

// Clase base de cualquier component
class Component
{
public:
	Component(Object* owner);
	virtual ~Component();

	// Returns a new copy of the Component
	virtual Component* clone(Object* newOwner) const;


protected:
	Object* owner;
};

