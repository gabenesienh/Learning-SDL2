#include "util.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "game.hpp"

using std::cin, std::cout, std::endl;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* gameSurface;

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow(
		"Snake",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		CELLS_HORIZONTAL*CELLS_SIZE,
		CELLS_VERTICAL*CELLS_SIZE,
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

	// Temporary surface, will be formatted into the game surface
	SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(
		winSurface->flags,
		CELLS_HORIZONTAL*CELLS_SIZE,
		CELLS_HORIZONTAL*CELLS_SIZE,
		winSurface->format->BitsPerPixel,
		winSurface->format->format
	);
	if (!temp) {
		cout << "Error creating game surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Convert game surface to window surface for faster blitting
	gameSurface = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!gameSurface) {
		cout << "Error formatting game surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	return true;
}

void kill() {
	for (auto gobj : gameObjects) {
		delete gobj;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}