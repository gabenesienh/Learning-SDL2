// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>

#include "events.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "util.hpp"

const float TARGET_FRAMERATE = 60;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

int main(int argc, char** argv) {
	if (!init()) return 1;

	debugMode = true;

	while (true) {
		deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;

		if (deltaTime < 1/TARGET_FRAMERATE) continue; // Framerate cap

		ticksLast = SDL_GetTicks64();

		if (!doEvents()) break;
		doGame();
		doRender();
	}

	kill();
	return 0;
}