#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <cmath>

#include "quadtree.hpp"
#include "util.hpp"

using std::max, std::abs;

bool isColorsSet = false;
Uint32 colorBackground;
Uint32 colorTree;
Uint32 colorPoint;

// Gives values to the color* variables
// Necessary since winSurface->format is null at compile time
void setColors() {
    if (isColorsSet) return;

    colorBackground = SDL_MapRGB(
        winSurface->format,
        0,
        0,
        0
    );

    colorTree = SDL_MapRGB(
        winSurface->format,
        255,
        127,
        63
    );

    colorPoint = SDL_MapRGB(
        winSurface->format,
        255,
        255,
        255
    );

    isColorsSet = true;
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

void drawRectangle(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1) {
    drawLine(surface, rendererRect, color, x0, y0, x1, y0);
    drawLine(surface, rendererRect, color, x1, y0, x1, y1);
    drawLine(surface, rendererRect, color, x1, y1, x0, y1);
    drawLine(surface, rendererRect, color, x0, y1, x0, y0);
}

void renderTree(QuadTree* tree, SDL_Rect& rendererRect) {
    if (tree == nullptr) return;

    setColors();

    /* -- Draw tree outline -- */

    rendererRect.w = 1;
    rendererRect.h = 1;

    if (tree->nw == nullptr) {
        drawRectangle(
            winSurface,
            rendererRect,
            colorTree,
            tree->boundary.center.x - tree->boundary.inradius,
            tree->boundary.center.y - tree->boundary.inradius,
            tree->boundary.center.x + tree->boundary.inradius,
            tree->boundary.center.y + tree->boundary.inradius
        );
    } else {
        renderTree(tree->nw, rendererRect);
        renderTree(tree->ne, rendererRect);
        renderTree(tree->sw, rendererRect);
        renderTree(tree->se, rendererRect);
    }

    /* -- Draw tree points -- */

    rendererRect.w = 3;
    rendererRect.h = 3;

    for (Point& point : tree->points) {
        rendererRect.x = point.x;
        rendererRect.y = point.y;
        SDL_FillRect(winSurface, &rendererRect, colorPoint);
    }

    rendererRect.w = 1;
    rendererRect.h = 1;
}

void clearScreen() {
    // Clear screen
    SDL_FillRect(winSurface, NULL, colorBackground);
}