#include "linealg.h"

#include <SDL2/SDL.h>
#include <cmath>

using std::abs, std::max;

// Draw a line to the cursor
void drawLine(SDL_Surface* surface, SDL_Rect* lineBrush, Uint32 color, Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1) {
	// Delta values (how far the cursor is from the anchor point)
	Sint32 dx = x1 - x0;
	Sint32 dy = y1 - y0;

	// Get the magnitude of the largest delta of the line
	// This makes it easy to adjust for all octants
	Sint32 step = max(abs(dx), abs(dy));

	for (int i = 0; i <= step; i++) {
		// Avoid division by 0 error
		if (step == 0) break;

		//double m = static_cast<double>(dy) / dx;

		// One of these will be 1, the other will be the slope
		double stepX = static_cast<double>(dx) / step;
		double stepY = static_cast<double>(dy) / step;

		lineBrush->x = x0 + i * stepX;
		lineBrush->y = y0 + i * stepY;

		// Adjust for brush size so it's centered
		lineBrush->x -= lineBrush->w/2;
		lineBrush->y -= lineBrush->h/2;

		SDL_FillRect(surface, lineBrush, color);
	}
}