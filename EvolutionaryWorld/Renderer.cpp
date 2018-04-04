#include "Renderer.h"

#include <iostream>
#include <GL\glew.h>

#include "Vector3.h"
#include "TimerManager.h"
#include "RenderingComponent.h"
#include "UIManager.h"

Renderer::Renderer()
{
	//Init SDL and GLEW
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Unable to initialize SDL" << std::endl;
	}

	//create the window
	window_SDL = SDL_CreateWindow("Evolutionary World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_OPENGL);
	//create the opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	context_SDL_GL = SDL_GL_CreateContext(window_SDL);

	// Set OPENGL attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
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

	projectionMatrix = Matrix4::createOrthoMatrix(1080, 720, 0.0, 1000);
	projectionMatrix.translate(Vector3(-1080 / 2.0, -720 / 2.0, 0.0)); // Put the camera centered so the bottom left corner is (0.0, 0.0)

	// Create UI manager
	uiManager = new UIManager(window_SDL);
}


Renderer::~Renderer()
{
	delete uiManager;

	//Delete the context
	SDL_GL_DeleteContext(context_SDL_GL);

	//Destroy the window
	SDL_DestroyWindow(window_SDL);

	//Stop SDL
	SDL_Quit();
}

void Renderer::handleInput()
{
	SDL_Event mEvent;
	while (SDL_PollEvent(&mEvent))
	{
		// Forward events to UIManager
		uiManager->processInputEvent(&mEvent);

		if (mEvent.type == SDL_MOUSEMOTION)
		{
			mouseX = mEvent.motion.x;
			mouseY = 720 - mEvent.motion.y;
		}

		if (mEvent.type == SDL_KEYDOWN)
		{
			if (mEvent.key.keysym.sym == SDLK_UP)
				TimerManager::getInstance()->worldSpeed++;
			if (mEvent.key.keysym.sym == SDLK_DOWN)
				TimerManager::getInstance()->worldSpeed--;
		}
	}
}

void Renderer::preRendering()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderFrame()
{
	for (auto& rComp : renderingComponents_list)
	{
		rComp->render(projectionMatrix);
	}

	// Render ui - by default tick is 60fps
	uiManager->processAndRenderWidgets(1.0f / 60.0f);
}

void Renderer::swapBuffers()
{
	SDL_GL_SwapWindow(window_SDL);
}

void Renderer::attachRenderingComponent(RenderingComponent * comp)
{
	renderingComponents_list.push_back(comp);
}

void Renderer::detachRenderingComponent(RenderingComponent * comp)
{
	for (auto it = renderingComponents_list.begin(); it != renderingComponents_list.end(); ++it)
	{
		if (*it == comp)
		{
			renderingComponents_list.erase(it);
			return;
		}
	}
}
