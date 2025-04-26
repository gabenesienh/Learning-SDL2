#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL.h>
#include <array>

using std::array;

// For saving keyboard inputs
extern array<bool, SDL_NUM_SCANCODES> keyStates;

// For saving keyboard inputs for one frame
extern array<bool, SDL_NUM_SCANCODES> keyStatesTap;

// Event loop
// Returns false when the window should close
bool doEvents();

#endif