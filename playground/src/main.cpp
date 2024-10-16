// A program with no specific purpose that exists for me to test random things
// Whenever I start developing a proper idea, it gets moved to its own folder and main.cpp file
// This file here will likely receive constant updates as I try new things

#include <SDL2/SDL.h>
#include <iostream>

int WinMain(int argc, char** argv) {
	// Initialize SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

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

	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 158, 222, 106));

	SDL_UpdateWindowSurface(window);
	system("pause");

	SDL_Surface* car = SDL_LoadBMP("../images/car.bmp");
	if (!car) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", "Error loading Car :(", NULL);
		std::cout << "Error loading BMP image: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Draw Car
	if (SDL_BlitSurface(car, NULL, winSurface, NULL) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", "Error when drawing Car...", NULL);
		std::cout << "Error displaying BMP image: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_UpdateWindowSurface(window);
	system("pause");

	// Exit

	SDL_FreeSurface(car); //forever free
	SDL_DestroyWindow(window);
	window = NULL;
	winSurface = NULL;
	SDL_Quit();

	return 0;
}