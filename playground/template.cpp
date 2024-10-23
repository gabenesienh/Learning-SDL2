// A basic template for my SDL2 apps to expand upon
// Do not compile! This does nothing!

#include <SDL2/SDL.h>
#include <iostream>

using std::cin, std::cout;

bool init();
void kill();

SDL_Window* window;
SDL_Surface* winSurface;

int main(int argc, char** argv) {
	if (!init()) return 1;

	// Program goes here

	kill();
	return 0;
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540, 0);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface) {
		cout << "Error getting surface: " << SDL_GetError() << std::endl;
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