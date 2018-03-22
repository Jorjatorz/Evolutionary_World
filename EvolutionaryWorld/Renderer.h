#pragma once

#include <SDL.h>

#include "Singleton.h"

// Singleton class
class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	void handleInput();
	void preRendering();
	void swapBuffers();

	int mouseX;
	int mouseY;

private:
	SDL_Window* window_SDL;
	SDL_GLContext context_SDL_GL;
};

