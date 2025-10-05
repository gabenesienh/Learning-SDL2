#include "quadtree.hpp"

#include <array>
#include <vector>

#include "objects.hpp"

using std::array;
using std::vector;

/* -- QuadTree -- */

// Constructors
QuadTree::QuadTree(AABB bounds)
    : bounds(bounds) {}

// Getters
AABB&               QuadTree::getBounds()          { return this->bounds; }
vector<GameObject*> QuadTree::getObjects() const   { return this->objects; }
array<QuadTree*, 4> QuadTree::getQuadrants() const { return this->quads; }

// Other methods
void QuadTree::clear() {
    // Delete all the objects this node holds
    for (auto gobj : this->objects) {
        delete(gobj);
    }

    // Delete the object pointers in this node
    this->objects.clear();

    // Recursively delete all objects contained in this node's quadrants
    // Also delete the quadrants themselves
    if (this->quads[0] != nullptr) {
        for (auto quad : this->getQuadrants()) {
            quad->clear();
            delete(quad);
            quad = nullptr;
        }
    }
}

void QuadTree::subdivide() {
    // For convenience
    AABB& thisBox = this->bounds;

    vec2 nwCenter = {
        thisBox.center.x - thisBox.halfWidth,
        thisBox.center.y - thisBox.halfHeight
    };
    vec2 neCenter = {
        thisBox.center.x + thisBox.halfWidth,
        thisBox.center.y - thisBox.halfHeight
    };
    vec2 swCenter = {
        thisBox.center.x - thisBox.halfWidth,
        thisBox.center.y + thisBox.halfHeight
    };
    vec2 seCenter = {
        thisBox.center.x + thisBox.halfWidth,
        thisBox.center.y + thisBox.halfHeight
    };

    this->quads[0] = new QuadTree(
        AABB(nwCenter, thisBox.halfWidth, thisBox.halfHeight)
    );
    this->quads[1] = new QuadTree(
        AABB(neCenter, thisBox.halfWidth, thisBox.halfHeight)
    );
    this->quads[2] = new QuadTree(
        AABB(swCenter, thisBox.halfWidth, thisBox.halfHeight)
    );
    this->quads[3] = new QuadTree(
        AABB(seCenter, thisBox.halfWidth, thisBox.halfHeight)
    );
}

int QuadTree::findFittingQuadrant(AABB& box) const {
    // For convenience
    const AABB& thisBox = this->bounds;

    bool fitsNorth;
    bool fitsSouth;
    bool fitsWest;
    bool fitsEast;

    // Check if the given box fits any possible halves of this node's bounds
    // In theory, no more than 2 of these should be true at the same time
    if (box.getTopY() >= thisBox.getTopY()
    &&  box.getBottomY() <= thisBox.center.y) {
        fitsNorth = true;
    }
    if (box.getTopY() >= thisBox.center.y
    &&  box.getBottomY() <= thisBox.getBottomY()) {
        fitsSouth = true;
    }
    if (box.getLeftX() >= thisBox.getLeftX()
    &&  box.getRightX() <= thisBox.center.x) {
        fitsWest = true;
    }
    if (box.getLeftX() >= thisBox.center.x
    &&  box.getRightX() <= thisBox.getRightX()) {
        fitsEast = true;
    }

    // If the object fits a vertical and a horizontal half, that's a quadrant
    // Return the this->quads index of the quadrant in question
    if (fitsNorth) {
        if (fitsWest) {
            return QUAD_NW;
        }
        if (fitsEast) {
            return QUAD_NE;
        }
    }
    if (fitsSouth) {
        if (fitsWest) {
            return QUAD_SW;
        }
        if (fitsEast) {
            return QUAD_SE;
        }
    }

    return -1;
}

void QuadTree::insert(GameObject* gobj) {
    // Does this node already have quadrants generated?
    if (this->quads[0] != nullptr) {
        int fitsIndex = this->findFittingQuadrant(gobj->getBounds());

        // Insert this gobj into a quadrant instead, if there's one that fits it
        if (fitsIndex != -1) {
            this->quads[fitsIndex]->insert(gobj);
            return;
        }
    }

    // No quads generated or it doesn't fit into any of them, so insert it into
    // this node
    this->objects.push_back(gobj);

    // Split this node if it's now above capacity
    if (this->objects.size() > QuadTree::BUCKET_CAPACITY) {
        if (this->quads[0] != nullptr) {
            this->subdivide();
        }

        /*
         * Move this node's objects into its newly-generated quadrants, if
         * they fit
         *
         * This looks like it should be a for loop. However, once an element is
         * erased from this->objects, the elements behind it get shifted forward
         * by one index, so the index should not increment in such a scenario
         */
        int i = 0;

        while (i < this->objects.size()) {
            int fitsIndex = this->findFittingQuadrant(this->objects[i]->getBounds());

            if (fitsIndex != -1) {
                this->quads[fitsIndex]->insert(this->objects[i]);
                this->objects.erase(this->objects.begin() + i);
                return;
            }

            i++;
        }
    }
}
vector<GameObject*> QuadTree::findPossibleCollisions(
    AABB& box,
    vector<GameObject*> acc
) const {
    // acc is an accumulator with all objects that could collide with this AABB

    // Recursively add objects from other quadrants this AABB fits into
    if (this->quads[0] != nullptr) {
        int index = this->findFittingQuadrant(box);

        if (index != -1) {
            acc = this->quads[index]->findPossibleCollisions(box, acc);
        }
    }

    // Add all objects from this current node
    for (GameObject* gobj : this->objects) {
        acc.push_back(gobj);
    }

    return acc;
}