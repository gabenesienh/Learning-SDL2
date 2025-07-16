#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL.h>

extern SDL_Event event;

// Event loop
// Returns false when the window should close
extern bool doEvents();

#endif