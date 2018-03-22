#pragma once

#include <functional>

class TimerManager;

//Class that represents a game independent timer
class Timer
{
	//Let TimerManager acces tick function
	friend class TimerManager;

public:
	~Timer();

	//Reset the timer
	void reset();
	//Pause the timer
	void pause();
	//Resume the timer (if paused)
	void resume();

	int getTime_milliseconds();


private:
	//************************************
	// Method:    Timer
	// FullName:  Timer::Timer
	// Access:    public 
	// Returns:   
	// Qualifier: Creates a new timer with all the parameters needed
	// Parameter: std::function<void(void)> funcToCall
	// Parameter: float timerDuration
	// Parameter: bool loop
	//************************************
	Timer(unsigned short int id, const std::function<void(void)>& funcToCall, float timerDuration, bool loop);
	Timer(unsigned short int id);
	Timer();


	//Function that updates clock delta time each frame. If the new deltaTime is equal to timerDuration it handles the function.
	//Returns TRUE if the timer has expired and needs to be deleted
	bool tick(int deltaTime);

	//Returns the id of the timer
	unsigned short int getId()
	{
		return _id;
	}


	float _deltaTime; //Elapsed time since start
	TimerManager* _timerManagerPtr;
	unsigned short int _id; //global id of the timer (given by the timermanager)
	std::function<void(void)> _funcToCall;
	float _timerDuration;
	bool _loop;
	bool _paused;
};

