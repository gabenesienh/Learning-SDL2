// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>

using std::cin, std::cout;
using std::int32_t;

bool init();
bool loop();
void kill();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* canvas;

SDL_Event event;

SDL_Rect* brush;
SDL_Rect* cursorBarTop;			// The individual cursor bars
SDL_Rect* cursorBarRight;		// It's definitely a hacky way of doing things
SDL_Rect* cursorBarBottom;		// But I haven't learned PNGs yet
SDL_Rect* cursorBarLeft;

// Mouse position (saves values from event.motion)
struct mousePos {
	int32_t x = 0;
	int32_t y = 0;
} mousePos;

bool mouseLeftPressed = false;

int main(int argc, char** argv) {
	if (!init()) return 1;

	brush = new SDL_Rect {0, 0, 5, 5};
	cursorBarTop = new SDL_Rect{0, 0, 2, 4};
	cursorBarRight = new SDL_Rect{0, 0, 4, 2};
	cursorBarBottom = new SDL_Rect{0, 0, 2, 4};
	cursorBarLeft = new SDL_Rect{0, 0, 4, 2};

	SDL_ShowCursor(SDL_DISABLE);

	while (loop()) {
		//SDL_Delay(17);
	}

	kill();
	return 0;
}

bool loop() {
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 199, 252, 106));
				}
				if (event.key.repeat != 0) {
					SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 255, 127, 127));
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0, 0, 0));
				}
				break;
			case SDL_MOUSEMOTION:
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;

				if (mouseLeftPressed) {
					brush->x = mousePos.x - (brush->w/2);
					brush->y = mousePos.y - (brush->h/2);

					SDL_FillRect(canvas, brush, SDL_MapRGB(canvas->format, 255, 255, 255));
				}

				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) mouseLeftPressed = true;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouseLeftPressed = false;
				break;
			case SDL_QUIT:
				return false;
		}
	}

	SDL_BlitSurface(canvas, NULL, winSurface, NULL);

	// Draw cursor by drawing each bar individually and with different offsets
	cursorBarTop->x = mousePos.x - cursorBarTop->w/2;
	cursorBarTop->y = mousePos.y - (cursorBarTop->h*2 + 1);

	cursorBarRight->x = mousePos.x + (cursorBarRight->w + 1);
	cursorBarRight->y = mousePos.y - cursorBarRight->h/2;

	cursorBarBottom->x = mousePos.x - cursorBarBottom->w/2;
	cursorBarBottom->y = mousePos.y + (cursorBarBottom->h + 1);

	cursorBarLeft->x = mousePos.x - (cursorBarLeft->w*2 + 1);
	cursorBarLeft->y = mousePos.y - cursorBarLeft->h/2;

	SDL_FillRect(winSurface, cursorBarTop, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, cursorBarRight, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, cursorBarBottom, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, cursorBarLeft, SDL_MapRGB(winSurface->format, 255, 255, 255));

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

	window = SDL_CreateWindow("Not Aseprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

	// Temporary canvas, will be formatted into the real canvas
	SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(
		winSurface->flags,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		winSurface->format->BitsPerPixel,
		winSurface->format->format
	);
	if (!temp) {
		cout << "Error creating canvas surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	canvas = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!canvas) {
		cout << "Error formatting canvas surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	SDL_FreeSurface(temp);

	return true;
}

// Quit SDL2
void kill() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}