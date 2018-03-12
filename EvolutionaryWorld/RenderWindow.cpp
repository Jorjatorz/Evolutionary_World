#include "RenderWindow.h"

#include <iostream>

#include <GL\glew.h>

RenderWindow::RenderWindow()
{
	//Init SDL and GLEW
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Unable to initialize SDL" << std::endl;
	}

	//create the window
	window_SDL = SDL_CreateWindow("Evolutionary World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_OPENGL);
	//create the opengl context
	context_SDL_GL = SDL_GL_CreateContext(window_SDL);

	// Set OPENGL attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(0); //Disable VSync

	glewExperimental = GL_TRUE;
	glewInit();

	//GET INFO
	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::cout << "OpenGL " << version << " initialized." << std::endl;
	std::cout << "(" << vendor << "; " << renderer << ")" << std::endl;
	std::cout << "GLSL version: " << glslVersion << std::endl;

	// Setup OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
}


RenderWindow::~RenderWindow()
{
	//Delete the context
	SDL_GL_DeleteContext(context_SDL_GL);

	//Destroy the window
	SDL_DestroyWindow(window_SDL);

	//Stop SDL
	SDL_Quit();
}

void RenderWindow::handleInput()
{
	SDL_Event mEvent;
	while (SDL_PollEvent(&mEvent))
	{
	}
}

void RenderWindow::preRendering()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderWindow::swapBuffers()
{
	SDL_GL_SwapWindow(window_SDL);
}
