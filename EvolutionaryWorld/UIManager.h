#pragma once

#include <unordered_map>
#include <string>

#include "Singleton.h"
#include "Shader.h"

class UIWidget;
struct SDL_Window;
union SDL_Event;
class Shader;

// Manager of all the widgets
class UIManager : public Singleton<UIManager>
{
public:
	UIManager(SDL_Window* window);
	~UIManager();

	// Event polling
	void processInputEvent(SDL_Event* event);

	// Process and render all widgets
	void processAndRenderWidgets(float deltaTime);

	// Registers widgets into the manager. This should only be called by UIWidget constructur/destructor
	void registerWidget(UIWidget* widget);
	void unregisterWidget(const UIWidget* widget);

	// Returns a widget
	UIWidget* getWidget(std::string name);

private:
	// Map with all the widgets
	std::unordered_map<std::string, UIWidget*> widgets_map;

	Shader uiShader;
	unsigned int vboBuffer;
	unsigned int elementBuffer;
	unsigned int fontTexture;
};

