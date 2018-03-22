#pragma once

#include <unordered_map>
#include <functional>

#include "Singleton.h"
#include "Timer.h"

//Class that manage and updates all the timers of the engine.
class TimerManager : public Singleton<TimerManager>
{
public:
	TimerManager();
	~TimerManager();
	
	//Return the total amount of time since start
	static unsigned int getTotalExecutionTime();
	//Return the last frame delta time
	int getDeltaTime()
	{
		return _deltaTimeLastFrame;
	}

	// Creates a norma timer
	Timer* addTimer()
	{
		Timer* t = new Timer(_nextId);

		_timersList.insert(std::make_pair(_nextId, t));
		_nextId++;

		return t;
	}

	//Creates a new timer given a member function, a duration of the timer and a boolean that is true if the timer loops
	template <class T, class U>
	Timer* addTimer(U&& functionClass, T&& function, float timerDuration, bool loop)
	{
		Timer* t = new Timer(_nextId, std::bind(function, functionClass), timerDuration, loop);

		_timersList.insert(std::make_pair(_nextId, t));
		_nextId++;

		return t;
	}
	//Deletes a timer
	void deleteTimer(Timer* timer);

	//Updates the tick of all timers
	///TODO make this function efficient (with a queue with only the unpaused timers etc)
	void tick(int deltaTime);

private:
	//Last frame delta time
	float _deltaTimeLastFrame;

	//Container of all the created timers. ID/TIMER
	std::unordered_map<unsigned short int, Timer*> _timersList;
	unsigned short int _nextId; //Next id that will be given to a new timer
};

