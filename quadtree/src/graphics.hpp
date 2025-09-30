#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>

#include "quadtree.hpp"

extern void drawLine(
    SDL_Surface* surface,
    SDL_Rect& rendererRect,
    Uint32 color,
    int x0,
    int y0,
    int x1,
    int y1
);
extern void drawRectangle(
    SDL_Surface* surface,
    SDL_Rect& rendererRect,
    Uint32 color,
    int x0,
    int y0,
    int x1,
    int y1
);

// Draws the tree and its points
extern void renderTree(QuadTree* tree, SDL_Rect& rendererRect);

// Fills the window surface with the background color
extern void clearScreen();

#endif