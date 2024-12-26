// A basic template for my SDL2 apps to expand upon
// Do not compile! This does nothing!

#include <SDL2/SDL.h>
#include <iostream>

using std::cin, std::cout;

bool init();
bool loop();
void kill();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

SDL_Window* window;
SDL_Surface* winSurface;

SDL_Event event;

// Uint64 ticksLast = 0;
// float deltaTime = 0; // In seconds

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	// deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;
	// ticksLast = SDL_GetTicks64();

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
		}
	}
	
	SDL_UpdateWindowSurface(window);
	return true;
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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