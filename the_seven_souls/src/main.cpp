// Garntale

//TODO: color windows individually
//TODO: add music
//TODO: intro animation

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <array>

using std::cin, std::cout, std::endl;
using std::min, std::sin, std::cos;
using std::array;

bool init();
bool loop();
void kill();

const float SPIN_SPEED_MULTIPLIER = 1.2;
const float RADIUS_MULTIPLIER = 0.65;
const int NUM_WINDOWS = 7;

SDL_Surface* loadImage(const char* imgPath);
void spinWindows(array<SDL_Window*, NUM_WINDOWS>& windows);

array<SDL_Window*, NUM_WINDOWS> windows;
array<SDL_Surface*, NUM_WINDOWS> winSurfaces;

SDL_DisplayMode display;

SDL_Event event;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// The center of the invisible ring windows spin around
// Set when initializing
struct {
	int x = 0;
	int y = 0;
} anchor;

// How far from the center the windows should be
// Either the display width or height, whichever is smallest
int spinRadius = 0;

// Where the window is currently supposed to be on the ring
// Loops back to zero at 2pi
double spinOffset = 0;

int main(int argc, char** argv) {
	if (!init()) return 1;
	
	SDL_Surface* image = loadImage("../img/car_heart.bmp");
	if (image == nullptr) return 1;

	for (int i = 0; i < NUM_WINDOWS; i++) {
		SDL_BlitScaled(image, NULL, winSurfaces[i], NULL);
		SDL_UpdateWindowSurface(windows[i]);
	}

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

	spinWindows(windows);

	spinOffset += deltaTime * SPIN_SPEED_MULTIPLIER;
	while (spinOffset >= 2*3.14) {
		spinOffset -= 2*3.14;
	}
	
	return true;
}

SDL_Surface* loadImage(const char* imgPath) {
	SDL_Surface* temp = SDL_LoadBMP(imgPath);
	if (!temp) {
		cout << "Error loading BMP: " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_Surface* img = SDL_ConvertSurface(temp, winSurfaces[0]->format, 0);
	if (!img) {
		cout << "Error converting BMP: " << SDL_GetError() << endl;
		return nullptr;
	}

	SDL_FreeSurface(temp);

	return img;
}

// Updates the position of the windows on the ring
void spinWindows(array<SDL_Window*, NUM_WINDOWS>& windows) {
	// How far apart each window should be from one another
	float spacing = 2*3.14 / (windows.size() - 1);

	// windows[0] is the middle window, so don't move it
	for (int i = 1; i < NUM_WINDOWS; i++) {
		// Displace the window based on its position in the array
		float windowOffset = spacing * i;
	
		// Calculate where the window should be on the ring
		int offsetX = sin(spinOffset + windowOffset) * spinRadius * RADIUS_MULTIPLIER;
		int offsetY = cos(spinOffset + windowOffset) * spinRadius * RADIUS_MULTIPLIER;
	
		SDL_SetWindowPosition(windows[i], anchor.x + offsetX, anchor.y - offsetY);
	}
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	if (SDL_GetCurrentDisplayMode(0, &display) != 0) {
		cout << "Error getting display mode info: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Adjust window size based on display
	int windowSize = min(display.w/5, display.h/5);

	for (int i = 0; i < NUM_WINDOWS; i++) {
		windows[i] = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize, windowSize, 0);
		if (!windows[i]) {
			cout << "Error creating window: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}
	
		winSurfaces[i] = SDL_GetWindowSurface(windows[i]);
		if (!winSurfaces[i]) {
			cout << "Error getting surface: " << SDL_GetError() << endl;
			system("pause");
			return false;
		}
	}

	// Center the ring on the screen
	anchor.x = display.w/2 - windowSize/2;
	anchor.y = display.h/2 - windowSize/2;

	spinRadius = min(display.w/2, display.h/2);

	return true;
}

// Quit SDL2
void kill() {
	for (int i = 0; i < NUM_WINDOWS; i++) {
		SDL_DestroyWindow(windows[i]);
	}
	SDL_Quit();
}