#include "RenderWindow.h"
#include "EWorld.h"

int main(int argc, char* argv[])
{
	RenderWindow render;
	EWorld world;

	const int generations_per_second = 1;
	short int deltaTime = 0;
	short int elapsedTime = 0;
	while (true)
	{
		deltaTime = SDL_GetTicks();

		render.handleInput();

		world.processPopulation();

		render.preRendering();
		world.renderPopulation();
		render.swapBuffers();

		deltaTime = SDL_GetTicks() - deltaTime;
		elapsedTime += deltaTime;
		if (elapsedTime < 1000 / generations_per_second)
		{
			SDL_Delay(1000 / generations_per_second - elapsedTime);
			elapsedTime = 0;
		}
	}

	return 0;
}