#pragma once

#include <iostream>
#include <string>
#include <cstdarg>

class FLog
{
public:
	FLog();
	~FLog();

	//Enumeration with the possible log outputs
	enum FLog_Type
	{
		INFO,
		WARNING,
		FAILURE
	};

	//Prints a text into the console, depending on the type it will show an info, warning or error text.
	FLog(FLog_Type lType, std::string mesage);

	//Prints a text with an argument into the console, depending on the type it will show an info, warning or error text.
	template <typename T>
	FLog(FLog_Type lType, std::string mesage, T arg)
	{
		std::va_list list;
		//Look for the correct type and print it
		if (lType == FLog_Type::INFO)
		{
			printInfo();
			printf(mesage.c_str(), arg);
			std::cout << std::endl;
		}
		else if (lType == FLog_Type::WARNING)
		{
			printWarning();
			printf(mesage.c_str(), arg);
			std::cout << std::endl;
		}
		else
		{
			printError();
			printf(mesage.c_str(), arg);
			std::cout << std::endl;
		}
	}

private:

	//Auxiliary functions to print the info,warning,error with color
	void printInfo();
	void printWarning();
	void printError();
	//Prints the curent hour-minutes-second
	void printCurrentTime();


	//Private copy constructor and assign operator
	FLog(const FLog&);
	FLog& operator=(const FLog&);
};

