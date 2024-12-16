// Bresenham's line algorithm
// Made for use in Worseprite

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
		// Draw a line to the cursor, one pixel at a time
		for (Sint32 pixelX = anchorPos.x; pixelX < mousePos.x; pixelX++) {
			// How far the cursor is from the anchor point
			Sint32 lineLengthX = mousePos.x - anchorPos.x;
			Sint32 lineLengthY = mousePos.y - anchorPos.y;

			// How much Y varies per X
			double ratio = static_cast<float>(lineLengthY) / lineLengthX;

			// Calculate the desired pixel Y value for every pixel X value
			Sint32 pixelY = anchorPos.y + (pixelX - anchorPos.x)*ratio;

			lineBrush->x = pixelX;
			lineBrush->y = pixelY;

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

	window = SDL_CreateWindow("Bresenham's line algorithm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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