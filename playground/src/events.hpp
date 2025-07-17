// Input events (e.g. keyboard presses, mouse movement, window closing)

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL.h>
#include <array>

using std::array;

// The current state of all keyboard keys
// true means pressed, false means not pressed
extern array<bool, SDL_NUM_SCANCODES> keyStates;

// Event loop
// Returns false when the window should close
extern bool doEvents();

#endif