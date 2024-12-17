// Line drawing algorithm
// Made for use in Worseprite

// TODO: implement bresenham's algorithm

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using std::cin, std::cout;
using std::abs, std::max;

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
				if (event.button.button == SDL_BUTTON_LEFT) {
					// Set the origin point for the drawn line
					anchorPos = {event.button.x, event.button.y};
					mousePressed = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					mousePressed = false;
				}
				break;
			case SDL_QUIT:
				return false;
		}
	}

	// Clear screen every loop
	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));

	if (mousePressed) {
		// Draw a line to the cursor

		// Start and end coordinates for the line
		Sint32 x0 = anchorPos.x;
		Sint32 y0 = anchorPos.y;
		Sint32 x1 = mousePos.x;
		Sint32 y1 = mousePos.y;

		// Delta values (how far the cursor is from the anchor point)
		Sint32 dx = x1 - x0;
		Sint32 dy = y1 - y0;

		// Get the magnitude of the largest delta of the line
		// This makes it easy to adjust for all octants
		Sint32 step = max(abs(dx), abs(dy));

		for (int i = 0; i <= step; i++) {
			// Avoid division by 0 error
			if (step == 0) break;

			//double m = static_cast<double>(dy) / dx;

			// One of these will be 1, the other will be the slope
			double stepX = static_cast<double>(dx) / step;
			double stepY = static_cast<double>(dy) / step;

			lineBrush->x = x0 + i * stepX;
			lineBrush->y = y0 + i * stepY;

			SDL_FillRect(winSurface, lineBrush, SDL_MapRGB(winSurface->format, 255, 255, 255));
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