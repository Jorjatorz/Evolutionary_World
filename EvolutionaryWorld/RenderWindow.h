#pragma once

#include <SDL.h>

class RenderWindow
{
public:
	RenderWindow();
	~RenderWindow();

	void handleInput();
	void preRendering();
	void swapBuffers();

private:
	SDL_Window* window_SDL;
	SDL_GLContext context_SDL_GL;
};

