#include "graphics.hpp"

#include <SDL2/SDL.h>

#include "game.hpp"
#include "util.hpp"

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

void doRender() {
	if (!isColorsSet) setColors();

	// Clear screen before drawing
	SDL_FillRect(gameSurface, NULL, blackColor);

	if (debugMode) {
		// Render object hitboxes
		for (GameObject* gobj : gameObjects) {
			rendererRect.w = gobj->getWidth();
			rendererRect.h = gobj->getHeight();
			rendererRect.x = gobj->getX() - rendererRect.w/2;
			rendererRect.y = gobj->getY() - rendererRect.h;
			SDL_FillRect(gameSurface, &rendererRect, debugHitboxColor);

			rendererRect.w = 1;
			rendererRect.h = 1;
			rendererRect.x = gobj->getX() - rendererRect.w/2;
			rendererRect.y = gobj->getY() - rendererRect.h;
			SDL_FillRect(gameSurface, &rendererRect, debugAnchorColor);
		}
	}

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
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

	isColorsSet = true;
}