#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <cmath>

#include "game.hpp"
#include "util.hpp"

using std::abs, std::max;

void setColors();

// Used for rendering game objects as solid rectangles in debug mode
// Width and height are adjusted per object while rendering
SDL_Rect rendererRect;

// Whether or not the necessary colors have been mapped yet
// setColors should not be called if this is true
bool isColorsSet = false;

Uint32 blackColor;

Uint32 debugHitboxColor;
Uint32 debugAnchorColor;
Uint32 debugDirectionColor;

void doRender() {
	if (!isColorsSet) setColors();

	// Clear screen before drawing
	SDL_FillRect(gameSurface, NULL, blackColor);

	// Debug info
	if (debugMode) {
		for (GameObject* gobj : gameObjects) {
			/* -- Render object hitbox -- */

			rendererRect.w = gobj->getWidth();
			rendererRect.h = gobj->getHeight();
			rendererRect.x = gobj->getX() - rendererRect.w/2;
			rendererRect.y = gobj->getY() - rendererRect.h;
			SDL_FillRect(gameSurface, &rendererRect, debugHitboxColor);

			/* -- Draw object's anchor point -- */

			rendererRect.w = 4;
			rendererRect.h = 4;
			rendererRect.x = gobj->getX() - rendererRect.w/2;
			rendererRect.y = gobj->getY() - rendererRect.h/2;
			SDL_FillRect(gameSurface, &rendererRect, debugAnchorColor);

			/* --
			Draw line from object's center to the direction they're facing
			-- */

			rendererRect.w = 1;
			rendererRect.h = 1;

			int centerX = gobj->getX() - rendererRect.w/2;
			int centerY = gobj->getY() - gobj->getHeight()/2 - rendererRect.h/2;

			// The endpoint of the line that will be drawn
			int targetX;
			int targetY;

			// Adjust endpoint based on object direction
			switch (gobj->getDirection()) {
				case DIR_LEFT:
					targetX = centerX - 25;
					targetY = centerY;
					break;
				case DIR_RIGHT:
					targetX = centerX + 25;
					targetY = centerY;
					break;
				default:
					targetX = centerX;
					targetY = centerY;
					break;
			}

			drawLine(
				gameSurface, rendererRect, debugDirectionColor,
				centerX, centerY,
				targetX, targetY
			);
		}
	}

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
}

void drawLine(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1) {
	// Deltas
	int dx = x1 - x0;
	int dy = y1 - y0;

	// The magnitude of the delta whose magnitude is the largest
	// This will help determine which octant the line is in
	double step = max(abs(dx), abs(dy));

	// How much X and Y should change with each "step" from (x0,y0) to (x1,y1)
	// Depending on the octant, one of these will always be 1 or -1
	double stepX = dx / step;
	double stepY = dy / step;

	for (int i = 0; i <= step; i++) {
		if (step == 0) break; // Avoid division by zero error

		rendererRect.x = x0 + stepX*i;
		rendererRect.y = y0 + stepY*i;

		SDL_FillRect(surface, &rendererRect, color);
	}
}

// Necessary due to surface formats being unknown before util.cpp is run
void setColors() {
	blackColor = SDL_MapRGB(
		gameSurface->format,
		0,
		0,
		0
	);

	debugHitboxColor = SDL_MapRGB(
		gameSurface->format,
		255,
		31,
		31
	);

	debugAnchorColor = SDL_MapRGB(
		gameSurface->format,
		31,
		255,
		255
	);

	debugDirectionColor = SDL_MapRGB(
		gameSurface->format,
		255,
		127,
		31
	);

	isColorsSet = true;
}