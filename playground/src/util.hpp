// SDL boilerplate and misc utilities

#ifndef UTIL_HPP
#define UTIL_HPP

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

extern SDL_Window* window;
extern SDL_Surface* winSurface;
extern SDL_Surface* gameSurface;

// Generic 2D vector
struct vec2 {
	double x;
	double y;

	bool operator==(const vec2& other) const;
	bool operator!=(const vec2& other) const;

	// Make into a unit vector
	void normalize();
};

// Initialize SDL
extern bool init();

// Quit SDL2
extern void kill();

#endif