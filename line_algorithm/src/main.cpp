// Line algorithm test app

#include <SDL2/SDL.h>
#include <iostream>
#include "linealg.h"

using std::cin, std::cout;

bool init();
bool loop();
void kill();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

SDL_Window* window;
SDL_Surface* winSurface;

SDL_Event event;

bool mousePressed = false;
struct {
	Sint32 x;
	Sint32 y;
} mousePos;
struct {
	Sint32 x;
	Sint32 y;
} anchorPos;

SDL_Rect* lineBrush = new SDL_Rect {0, 0, 1, 1};

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_MOUSEMOTION:
				mousePos = {event.button.x, event.button.y};
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						// Set the origin point for the drawn line
						anchorPos = {event.button.x, event.button.y};
						mousePressed = true;
						break;
					case SDL_BUTTON_RIGHT:
						lineBrush->w = 5;
						lineBrush->h = 5;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mousePressed = false;
						break;
					case SDL_BUTTON_RIGHT:
						lineBrush->w = 1;
						lineBrush->h = 1;
						break;
				}
				break;
			case SDL_QUIT:
				return false;
		}
	}

	// Clear screen every loop
	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));

	if (mousePressed) {
		drawLine(winSurface, lineBrush, SDL_MapRGB(winSurface->format, 255, 255, 255), anchorPos.x, anchorPos.y, mousePos.x, mousePos.y);
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

	window = SDL_CreateWindow("Line Drawing Algorithm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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