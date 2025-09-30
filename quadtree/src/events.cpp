#include "events.hpp"

#include <SDL2/SDL.h>

#include "util.hpp"

SDL_Event event;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// Event loop
bool doEvents() {
    deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;

    if (deltaTime < 1/TARGET_FRAMERATE) return true; // Framerate cap

    ticksLast = SDL_GetTicks64();

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
        }
    }
    
    SDL_UpdateWindowSurface(window);
    return true;
}