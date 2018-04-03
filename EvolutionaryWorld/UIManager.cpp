#include "UIManager.h"

#include "UIWidget.h"
#include "FLog.h"

UIManager::UIManager()
{
}


UIManager::~UIManager()
{
}

void UIManager::registerWidget(UIWidget* widget)
{
	if (widgets_map.find(widget->getName()) == widgets_map.end())
	{
		widgets_map.insert(std::make_pair(widget->getName(), widget));
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name already exists.");
	}
}

void UIManager::unregisterWidget(const UIWidget * widget)
{
	if (widgets_map.find(widget->getName()) != widgets_map.end())
	{
		widgets_map.erase(widget->getName());
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name doesn't exists, can't be removed.");
	}
}

UIWidget * UIManager::getWidget(std::string name)
{
	auto it = widgets_map.find(name);
	if (it != widgets_map.end())
	{
		return it->second;
	}
	else
	{
		FLog(FLog::FAILURE, "A widget with that name doesn't exists.");
	}
}
