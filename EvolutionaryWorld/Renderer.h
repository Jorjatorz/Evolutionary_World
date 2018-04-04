#pragma once

#include <SDL.h>
#include <vector>

#include "Singleton.h"
#include "Matrix4.h"
class RenderingComponent;
class UIManager;

// Singleton class
class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	void handleInput();
	void preRendering();
	void renderFrame();
	void swapBuffers();

	//Attachs a rendering component so it will be processed in each draw call
	void attachRenderingComponent(RenderingComponent* comp);

	//Detachs a rendering component so it will NOT be processed in each draw call
	void detachRenderingComponent(RenderingComponent* comp);

	int mouseX;
	int mouseY;

private:
	SDL_Window* window_SDL;
	SDL_GLContext context_SDL_GL;

	std::vector<RenderingComponent*> renderingComponents_list;

	Matrix4 projectionMatrix;

	UIManager* uiManager;
};

