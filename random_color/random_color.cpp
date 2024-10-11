// First SDL2 project!!
// This one just displays a random color every time you launch it.
// Launch with -epilepsymode to burn your eyes

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <cstring>

int main(int argc, char** argv) {
	// Initialize everything

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Random Color", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	SDL_Surface* winSurface = SDL_GetWindowSurface(window);

	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	// The actual program

	std::uniform_int_distribution<int> distRGB(0, 255);
	std::mt19937 rng(std::random_device{}());

	while (true) {
		SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, distRGB(rng), distRGB(rng), distRGB(rng)));
		SDL_UpdateWindowSurface(window);

		if (!(argc > 1 && std::strcmp(argv[1], "-epilepsymode") == 0)) break;
	}

	system("pause");
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}