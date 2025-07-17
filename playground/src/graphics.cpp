#include "graphics.hpp"

#include <SDL2/SDL.h>

#include "game.hpp"
#include "util.hpp"

void setColors();

// Used for rendering game objects as solid rectangles
// Width and height are adjusted per object while rendering
// Currently exists for debugging
SDL_Rect rendererRect;

// Whether or not the necessary colors have been mapped yet
// setColors should not be called if this is true
bool isColorsSet = false;

// The color used with rendererRect
Uint32 debugColor;

void doRender() {
	if (!isColorsSet) setColors();

	for (GameObject* gobj : gameObjects) {
		rendererRect.x = gobj->getX();
		rendererRect.y = gobj->getY();
		rendererRect.w = gobj->getWidth();
		rendererRect.h = gobj->getHeight();

		SDL_FillRect(gameSurface, &rendererRect, debugColor);
	}

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
}

// Necessary due to surface formats being unknown before util.cpp is run
void setColors() {
	debugColor = SDL_MapRGB(
		gameSurface->format,
		255,
		0,
		0
	);
}