#pragma once

#include <assert.h>

#include <typeinfo>

#include "FLog.h"

/*
* Base class. It applies the Singleton pattern to subclasses.
*/

template <class T>
class Singleton
{
private:
	Singleton(const Singleton<T> &); 
	Singleton& operator=(const Singleton<T> &);

protected:
	static T* _singleton; //static instance

public:
	Singleton(void) //Default constructor that is in charge of saving just one reference of the class
	{
		if (_singleton)
		{
			FLog(FLog::FAILURE, "Singleton class already instantiated: %s", typeid(T).name());
			assert(0); //just created once
		}
		_singleton = static_cast<T*>(this); //allocate the singleton
	}

	~Singleton(void) //Default destructor that is in charge of erasing the singleton reference
	{
		if (!_singleton)
		{
			FLog(FLog::FAILURE, "Singleton class not instantiated, can't delete it: %s", typeid(T).name());
			assert(0); //just created once
		}

		_singleton = 0;
	}

	static T* const getInstance(void) //Return a pointer to the singleton object
	{
		if (!_singleton)
		{
			FLog(FLog::FAILURE, "Singleton class not instantiated, can't return it: %s", typeid(T).name());
			assert(0); //just created once
		}
		return _singleton;
	}


};

//initiate the singleton
template <typename T>
T* Singleton<T>::_singleton = 0;