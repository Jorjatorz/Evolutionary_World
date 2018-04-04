#pragma once

#include <string>

// Base class for all the different widgets
class UIWidget
{
public:
	UIWidget(std::string name);
	virtual ~UIWidget();

	// In this function the logic and rendering of the widget is implemented
	virtual void render() = 0;

	std::string getName() const;

private:
	std::string name;
};

