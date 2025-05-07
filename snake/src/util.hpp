#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>

const int CELLS_SIZE = 15;
const int CELLS_HORIZONTAL = 16;
const int CELLS_VERTICAL = 16;

extern SDL_Window* window;
extern SDL_Surface* winSurface;
extern SDL_Surface* gameSurface;

// Initialize SDL
extern bool init();

// Quit SDL2
extern void kill();

#endif