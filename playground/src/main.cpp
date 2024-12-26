// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using std::cin, std::cout;
using std::min, std::sin, std::cos;

bool init();
bool loop();
void kill();

void spinWindow(SDL_Window* window);

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 320;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_DisplayMode display;

SDL_Event event;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// The center of the invisible circle windows spin around
struct {
	int x = 0;
	int y = 0;
} anchor;

// Either the display width or height, whichever is smallest
int spinRadius = 0;

// Where the window is currently supposed to be on the invisible circle
// Loops back to zero at 2pi
double spinOffset = 0;

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;
	ticksLast = SDL_GetTicks64();

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
		}
	}

	spinWindow(window);

	spinOffset += deltaTime;
	while (spinOffset >= 2*3.14) {
		spinOffset -= 2*3.14;
	}
	
	SDL_UpdateWindowSurface(window);
	return true;
}

// Updates the position of a window on the invisible circle
void spinWindow(SDL_Window* window) {
	int offsetX = sin(spinOffset) * spinRadius;
	int offsetY = cos(spinOffset) * spinRadius;

	SDL_SetWindowPosition(window, anchor.x + offsetX, anchor.y + offsetY);
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Speen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

	if (SDL_GetCurrentDisplayMode(0, &display) != 0) {
		cout << "Error getting display mode info: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	anchor.x = display.w/2 - WINDOW_WIDTH/2;
	anchor.y = display.h/2 - WINDOW_HEIGHT/2;

	spinRadius = min(display.w/2, display.h/2);

	return true;
}

// Quit SDL2
void kill() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}