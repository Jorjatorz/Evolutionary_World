#pragma once

#include <string>

// Base class for all the different widgets
class UIWidget
{
public:
	UIWidget(std::string name);
	virtual ~UIWidget();

	std::string getName() const;

private:
	std::string name;
};

