// Quadtree implementation test

#include <SDL2/SDL.h>
#include <array>
#include <random>

#include "events.hpp"
#include "graphics.hpp"
#include "quadtree.hpp"
#include "util.hpp"

using std::array;

// How many points to populate with
const int POINT_COUNT = 250;

// Rect used for drawing things on the window surface
SDL_Rect rendererRect = {0, 0, 1, 1};

// The top-most node of the quadtree
QuadTree root = QuadTree(
    AABB(
        Point(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
        WINDOW_HEIGHT/2
    )
);

// For generating random point coordinates that are inside root's boundary
std::uniform_int_distribution<int> distRoot(0, root.boundary.inradius*2);
std::mt19937 rng(std::random_device{}());

int main(int argc, char** argv) {
    if (!init()) return 1;

    // Populate the tree on launch
    for (int i = 0; i < POINT_COUNT; i++) {
        root.insert(
            Point(
                root.boundary.center.x - root.boundary.inradius + distRoot(rng),
                root.boundary.center.y - root.boundary.inradius + distRoot(rng)
            )
        );
    }

    while (true) {
        if (!doEvents()) break;
        clearScreen();
        renderTree(&root, rendererRect);
    }

    kill();
    return 0;
}