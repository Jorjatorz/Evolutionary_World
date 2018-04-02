#include "Renderer.h"
#include "EWorld_food.h"
#include "TimerManager.h"

int main(int argc, char* argv[])
{
	TimerManager time_manager;
	Renderer render;
	EWorld* world = new EWorld_food();

	short int deltaTime = 0;
	short int elapsedTime = 0;
	short int frame_start = 0;
	while (true)
	{
		render.handleInput();

		frame_start = time_manager.getTotalExecutionTime();
		time_manager.tick(deltaTime); // Update timers

		world->tickWorld();

		render.preRendering();
		render.renderFrame();
		render.swapBuffers();

		deltaTime = time_manager.getTotalExecutionTime() - frame_start;
		//if (deltaTime < 1000 / 60.0f) // 60 fps
		//{
		//	SDL_Delay(1000 / 60.0f - deltaTime);
		//	deltaTime = 1000 / 60.0f;
		//}
	}

	delete world;

	return 0;
}