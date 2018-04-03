#include "UIWidget.h"

#include "UIManager.h"

UIWidget::UIWidget(std::string name)
	:name(name)
{
	UIManager::getInstance()->registerWidget(this);
}


UIWidget::~UIWidget()
{
	UIManager::getInstance()->unregisterWidget(this);
}

std::string UIWidget::getName() const
{
	return name;
}
