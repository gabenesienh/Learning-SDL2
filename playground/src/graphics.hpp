// Rendering logic

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>

// Draws a frame
extern void doRender();

// Draw a straight line from (x0,y0) to (x1,y1)
// Parameters are screen coordinates rather than game coordinates
extern void drawLine(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1);

#endif