// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>

using std::cin, std::cout;

bool init();
bool loop();
void kill();

SDL_Window* window;
SDL_Surface* winSurface;

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (loop()) {
		//SDL_Delay(17);
	}

	kill();
	return 0;
}

bool loop() {
	SDL_Event ev;

	while (SDL_PollEvent(&ev) != 0) {
		switch (ev.type) {
			case SDL_KEYDOWN:
				if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 199, 252, 106));
				}
				break;
			case SDL_KEYUP:
				if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));
				}
				break;
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