#include "util.hpp"

#include <SDL2/SDL.h>
#include <iostream>

using std::cin, std::cout, std::endl;

SDL_Window* window;
SDL_Surface* winSurface;

// Initialize SDL
bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    window = SDL_CreateWindow(
        "Quadtree",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window) {
        cout << "Error creating window: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        cout << "Error getting surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    return true;
}

// Quit SDL2
void kill() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}