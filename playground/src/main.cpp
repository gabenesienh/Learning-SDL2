// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using std::cin, std::cout, std::endl;
using std::min, std::sin, std::cos;

bool init();
bool loop();
void kill();

SDL_Surface* loadImage(const char* imgPath);
void spinWindow(SDL_Window* window);

const int WINDOW_SIZE = 240;
const float SPIN_SPEED_MULTIPLIER = 1;
const float RADIUS_MULTIPLIER = 0.8;

SDL_Window* window;
SDL_Surface* winSurface;

SDL_DisplayMode display;

SDL_Event event;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// The center of the invisible ring windows spin around
struct {
	int x = 0;
	int y = 0;
} anchor;

// Either the display width or height, whichever is smallest
int spinRadius = 0;

// Where the window is currently supposed to be on the ring
// Loops back to zero at 2pi
double spinOffset = 0;

int main(int argc, char** argv) {
	if (!init()) return 1;
	
	SDL_Surface* image = loadImage("../img/car.bmp");
	if (image == nullptr) return 1;

	SDL_BlitScaled(image, NULL, winSurface, NULL);

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;

	// Cap the framerate at 60 FPS so it doesn't crash the GPU driver (lmao)
	if (deltaTime < 0.017) return true;

	ticksLast = SDL_GetTicks64();

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
		}
	}

	spinWindow(window);

	spinOffset += deltaTime * SPIN_SPEED_MULTIPLIER;
	while (spinOffset >= 2*3.14) {
		spinOffset -= 2*3.14;
	}
	
	SDL_UpdateWindowSurface(window);
	return true;
}

SDL_Surface* loadImage(const char* imgPath) {
	SDL_Surface* temp = SDL_LoadBMP(imgPath);
	if (!temp) {
		cout << "Error loading BMP: " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_Surface* img = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!img) {
		cout << "Error converting BMP: " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_FreeSurface(temp);

	return img;
}

// Updates the position of a window on the ring
void spinWindow(SDL_Window* window) {
	int offsetX = sin(spinOffset) * spinRadius * RADIUS_MULTIPLIER;
	int offsetY = cos(spinOffset) * spinRadius * RADIUS_MULTIPLIER;

	SDL_SetWindowPosition(window, anchor.x + offsetX, anchor.y - offsetY);
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Speen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE, WINDOW_SIZE, 0);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface) {
		cout << "Error getting surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	if (SDL_GetCurrentDisplayMode(0, &display) != 0) {
		cout << "Error getting display mode info: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Center the ring on the screen
	anchor.x = display.w/2 - WINDOW_SIZE/2;
	anchor.y = display.h/2 - WINDOW_SIZE/2;

	spinRadius = min(display.w/2, display.h/2);

	return true;
}

// Quit SDL2
void kill() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}