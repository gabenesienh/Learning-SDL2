#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <cmath>

using std::max, std::abs;

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

void drawRectangle(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1) {
    drawLine(surface, rendererRect, color, x0, y0, x1, y0);
    drawLine(surface, rendererRect, color, x1, y0, x1, y1);
    drawLine(surface, rendererRect, color, x1, y1, x0, y1);
    drawLine(surface, rendererRect, color, x0, y1, x0, y0);
}