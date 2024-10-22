// My first attempt at loading a bitmap image into a window
// For extra silly, run with parameters -wide or -tall
// WARNING: -xwide is FAR TOO SILLY and should be used with caution

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <array>
#include <cstring>

using std::cin, std::cout;
using std::string;
using std::array;

bool init(int& argc, char**& argv);
void kill();
bool load();

SDL_Window* window;
SDL_Surface* winSurface;

SDL_Surface* car;

int main(int argc, char** argv) {
	if (!init(argc, argv)) return 1;
	if (!load()) return 1;

	// Draw Car (stretched if necessary)
	if (SDL_BlitScaled(car, NULL, winSurface, &winSurface->clip_rect) < 0) {
		string message = "Error when drawing Car...\n" + string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", message.c_str(), NULL);
		cout << "Error displaying BMP image: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);

	kill();
	return 0;
}

// Load Car
bool load() {
	SDL_Surface* temp = SDL_LoadBMP("../images/car.bmp");
	if (!temp) {
		string message = "Error loading Car :C\n" + string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", message.c_str(), NULL);
		cout << "Error loading BMP image: " << SDL_GetError() << std::endl;
		return false;
	}

	car = SDL_ConvertSurface(temp, winSurface->format, 0);

	SDL_FreeSurface(temp);

	if (!car) {
		string message = "Error converting surface!11!1\n" + string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error", message.c_str(), NULL);
		cout << "Error converting surface: " << SDL_GetError() << std::endl;
		return false;
	}

	// car->format->format = 390076419
	//                       0x17401803
	//                       SDL_PIXELFORMAT_BGR24

	// winSurface->format->format = 370546692
	//                              0x16161804
	//                              SDL_PIXELFORMAT_XRGB8888

	return true;
}

// Initialize SDL
bool init(int& argc, char**& argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	array<int, 2> windowSize = {256, 256};

	if (argc > 1) {
		if (std::strcmp(argv[1], "-wide") == 0) {
			windowSize[0] = 512;
		} else if (std::strcmp(argv[1], "-xwide") == 0) {
			windowSize[0] = 1024;
		} else if (std::strcmp(argv[1], "-tall") == 0) {
			windowSize[1] = 512;
		}
	}

	window = SDL_CreateWindow("Car!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize[0], windowSize[1], 0);
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
	SDL_FreeSurface(car); //forever free

	SDL_DestroyWindow(window);
	SDL_Quit();
}