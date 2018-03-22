#include "TimerManager.h"

#include "FLog.h"

#include <SDL.h>
#include <queue>

TimerManager::TimerManager()
	:_nextId(0),
	_deltaTimeLastFrame(0)
{
}


TimerManager::~TimerManager()
{
	for (auto &it : _timersList)
	{
		delete it.second;
	}
	_timersList.clear();
}

void TimerManager::deleteTimer(Timer* timer)
{
	auto it = _timersList.find(timer->getId());

	if (it != _timersList.end())
	{
		Timer* t = it->second;

		_timersList.erase(it);
		delete t;
	}
	else
	{
		FLog(FLog::FAILURE, "Timer with id %u doesn't exists", timer->getId());
	}
}

void TimerManager::tick(int deltaTime)
{
	//Update delta time
	_deltaTimeLastFrame = deltaTime;

	//Queue to store the timers that will be deleted after the tick
	std::queue<int> timersToErase;
	//Tick all timers
	for (auto const &it : _timersList)
	{
		if (it.second->tick(deltaTime))
		{
			timersToErase.push(it.second->getId());
		}
	}
	//Delete the timers that have ended
	while (!timersToErase.empty())
	{
		_timersList.erase(timersToErase.front());
		timersToErase.pop();
	}
}

unsigned int TimerManager::getTotalExecutionTime()
{
	return SDL_GetTicks();
}
