#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>

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

#endif