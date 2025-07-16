#include "events.hpp"

#include <SDL2/SDL.h>

SDL_Event event;

bool doEvents() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
		}
	}
	
	return true;
}