// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <cstring>

int main(int argc, char** argv) {
	// Initialize SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	std::array<int, 2> windowSize = {256, 256};

	if (argc > 1) {
		if (std::strcmp(argv[1], "-wide") == 0) {
			windowSize[0] = 512;
		} else if (std::strcmp(argv[1], "-xwide") == 0) {
			windowSize[0] = 1024;
		} else if (std::strcmp(argv[1], "-tall") == 0) {
			windowSize[1] = 512;
		}
	}

	SDL_Window* window = SDL_CreateWindow("Car!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize[0], windowSize[1], 0);

	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	SDL_Surface* winSurface = SDL_GetWindowSurface(window);

	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	// Program begins here

	SDL_Surface* car = SDL_LoadBMP("../images/car.bmp");
	if (!car) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", "Error loading Car :(", NULL);
		std::cout << "Error loading BMP image: " << SDL_GetError() << std::endl;
		return 1;
	}

	// car->format->format = 390076419
	//                       0x17401803
	//                       SDL_PIXELFORMAT_BGR24

	// winSurface->format->format = 370546692
	//                              0x16161804
	//                              SDL_PIXELFORMAT_XRGB8888

	// I could convert the image to the window format to be slightly more optimized
	// But like... it's a single static image...

	// Draw Car (stretched if necessary)
	if (SDL_BlitScaled(car, NULL, winSurface, &winSurface->clip_rect) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", "Error when drawing Car...", NULL);
		std::cout << "Error displaying BMP image: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);

	// Exit

	SDL_FreeSurface(car); //forever free
	SDL_DestroyWindow(window);
	window = NULL;
	winSurface = NULL;
	SDL_Quit();

	return 0;
}