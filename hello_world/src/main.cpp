#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char** argv) {
	// Initialize SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	// Program begins here

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello World", "Hello World!", NULL);

	SDL_Quit();

	return 0;
}