#pragma once

#include <SDL.h>

#include "Singleton.h"

// Singleton class
class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	virtual ~Renderer();

	void handleInput();
	void preRendering();
	void swapBuffers();

private:
	SDL_Window* window_SDL;
	SDL_GLContext context_SDL_GL;
};

