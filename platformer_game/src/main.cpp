// Platformer game

#include <SDL2/SDL.h>

#include "events.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "util.hpp"

const double TARGET_FRAMERATE = 60;

Uint64 ticksLast = 0;
double deltaTime = 0; // In seconds

int main(int argc, char** argv) {
	if (!init()) return 1;

	debugMode = true;

	while (true) {
		deltaTime = static_cast<double>(SDL_GetTicks64() - ticksLast)/1000;

		if (deltaTime < 1/TARGET_FRAMERATE) continue; // Framerate cap

		ticksLast = SDL_GetTicks64();

		if (!doEvents()) break;
		doGame();
		doRender();
	}

	kill();
	return 0;
}