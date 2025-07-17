#include "events.hpp"

#include <SDL2/SDL.h>
#include <array>

using std::array;

array<bool, SDL_NUM_SCANCODES> keyStates = {false};

SDL_Event event;

bool doEvents() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_KEYDOWN:
				keyStates[event.key.keysym.scancode] = true;

				break;
			case SDL_KEYUP:
				keyStates[event.key.keysym.scancode] = false;

				break;
			case SDL_QUIT:
				return false;
		}
	}
	
	return true;
}