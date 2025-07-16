#include "graphics.hpp"

#include <SDL2/SDL.h>

#include "util.hpp"

void doRender() {
	SDL_UpdateWindowSurface(window);
}