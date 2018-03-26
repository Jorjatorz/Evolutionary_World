#pragma once

#include <vector>

#include "FTransform.h"
#include "Component.h"

// Base class representing an object in the program
class Object
{
public:
	Object();
	Object(const Object& other);
	virtual ~Object();

	// Adds a components to the Actor (it doesnt check for duplicates)
	template <typename T>
	T* addComponent();

	// Remove a components given its class
	template <typename T>
	void removeComponentByClass();

	// Returns a component given its class
	template <typename T>
	T* getComponentByClass();

	FTransform* getTransform_pointer();

protected:
	FTransform transform;

	std::vector<Component*> components_list;
};

template<typename T>
inline T * Object::addComponent()
{
	//Create the component (it will automatically register into the actor)
	T* newComponent = new T(this);
	components_list.push_back(newComponent);

	return newComponent;
}

template<typename T>
inline void Object::removeComponentByClass()
{
	for (auto it = components_list.begin(); it != components_list.end(); ++it)
	{
		if (dynamic_cast<T*>(*it) != nullptr)
		{
			delete *it;
			components_list.erase(it);
			return;
		}
	}
}

template<typename T>
inline T* Object::getComponentByClass()
{
	T* toReturn = nullptr;
	for (auto it = components_list.begin(); it != components_list.end(); ++it)
	{
		T* elem = dynamic_cast<T*>(*it);
		if (elem != nullptr)
		{
			toReturn = elem;
			break;
		}
	}

	return toReturn;
}
