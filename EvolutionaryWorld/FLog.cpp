#include "FLog.h"

#include <chrono>
#include <Windows.h>

FLog::FLog(FLog_Type lType, std::string mesage)
{
	//Look for the correct type and print it
	if (lType == FLog_Type::INFO)
	{
		printInfo();
		std::cout << mesage << std::endl;
	}
	else if (lType == FLog_Type::WARNING)
	{
		printWarning();
		std::cout << mesage << std::endl;
	}
	else
	{
		printError();
		std::cout << mesage << std::endl;
	}
}


FLog::~FLog()
{
}

void FLog::printInfo()
{
	std::cout << "[";

	//Change the color
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 11); //Blue

	std::cout << "INFO";	

	std::cout << " - ";
	printCurrentTime();

	//Change the color back again
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15); //White

	std::cout << "] ";
}
void FLog::printWarning()
{
	std::cout << "[";

	//Change the color
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 14); //Light Yellow

	std::cout << "WARNING";

	std::cout << " - ";
	printCurrentTime();

	//Change the color back again
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15); //White

	std::cout << "] ";
}
void FLog::printError()
{
	std::cout << "[";

	//Change the color
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 12); //Light red

	std::cout << "ERROR";

	std::cout << " - ";
	printCurrentTime();

	//Change the color back again
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15); //White

	std::cout << "] ";
}

void FLog::printCurrentTime()
{
	//Get the it is printed
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	//auto currentTime = *localtime(&tt);

	//std::cout << currentTime.tm_hour << ":" << currentTime.tm_min << ":" << currentTime.tm_sec;
}
