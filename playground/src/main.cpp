// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

//TODO: better screen clear
//TODO: change background color
//TODO: custom background image
//TODO: color picker (brush)
//TODO: color picker (background)
//TODO: brush size slider
//TODO: remap scroll wheel function

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>
#include <array>

using std::cin, std::cout;
using std::int32_t;
using std::array;

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

array<array<int, 3>, 10> colors = {{
	{235, 64, 52},		// Red
	{245, 124, 59},		// Orange
	{245, 242, 73},		// Yellow
	{50, 168, 58},		// Green
	{66, 135, 245},		// Blue
	{52, 57, 201},		// Indigo
	{145, 75, 242},		// Purple
	{252, 96, 185}, 	// Pink
	{127, 127, 127},	// Gray
	{255, 255, 255}		// White
}};

bool mouseLeftPressed = false;
bool mouseRightPressed = false;
int activeColor = 9;

int main(int argc, char** argv) {
	if (!init()) return 1;

	brush = new SDL_Rect {0, 0, 5, 5};

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
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_SPACE:
						if (event.key.repeat == 0) {
							SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 199, 252, 106));
						} else {
							SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 255, 127, 127));
						}
						break;
					default:
						// Pick colors with num keys
						// Each scancode index is equal to the previous but incremented by 1
						// So this approach should be safe
						if (event.key.keysym.scancode >= SDL_SCANCODE_1
						&&  event.key.keysym.scancode <= SDL_SCANCODE_0) {
							activeColor = event.key.keysym.scancode - SDL_SCANCODE_1;
						}
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0, 0, 0));
				}
				break;
			case SDL_MOUSEMOTION:
				brush->x = event.motion.x - (brush->w/2);
				brush->y = event.motion.y - (brush->h/2);

				if (mouseLeftPressed && mouseRightPressed) {
					// Cancel out M1 + M2
					mouseLeftPressed = false;
					mouseRightPressed = false;
				}

				if (mouseLeftPressed) {
					// Paint the canvas
					int r = colors[activeColor][0];
					int g = colors[activeColor][1];
					int b = colors[activeColor][2];
					SDL_FillRect(canvas, brush, SDL_MapRGB(canvas->format, r, g, b));
				} else if (mouseRightPressed) {
					// Erase the canvas (by painting the background color)
					SDL_FillRect(canvas, brush, SDL_MapRGB(canvas->format, 0, 0, 0));
				}

				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftPressed = true;
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightPressed = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftPressed = false;
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightPressed = false;
						break;
				}
				break;
			case SDL_MOUSEWHEEL:
				brush->x = event.wheel.mouseX - (brush->w/2);
				brush->y = event.wheel.mouseY - (brush->h/2);
				brush->w += -event.wheel.y;
				brush->h += -event.wheel.y;
				break;
			case SDL_QUIT:
				return false;
		}
	}

	SDL_BlitSurface(canvas, NULL, winSurface, NULL);

	// Draw cursor by drawing each bar individually and with different offsets
	SDL_FillRect(winSurface, new SDL_Rect {brush->x, brush->y - 2, brush->w, 2}, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x + brush->w, brush->y, 2, brush->h}, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x, brush->y + brush->h, brush->w, 2}, SDL_MapRGB(winSurface->format, 255, 255, 255));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x - 2, brush->y, 2, brush->h}, SDL_MapRGB(winSurface->format, 255, 255, 255));

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