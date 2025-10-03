#include "quadtree.hpp"

#include <array>
#include <vector>

#include "objects.hpp"

using std::array;
using std::vector;

/* -- AABB -- */

// Constructors
AABB::AABB() {
    this->center = {0, 0};
    this->halfWidth = 0;
    this->halfHeight = 0;
}
AABB::AABB(vec2 center, double halfWidth, double halfHeight) {
    this->center = center;
    this->halfWidth = halfWidth;
    this->halfHeight = halfHeight;
}

// Getters
vec2   AABB::getCenter() const     { return this->center; }
double AABB::getHalfWidth() const  { return this->halfWidth; }
double AABB::getHalfHeight() const { return this->halfHeight; }

double AABB::getTopY() const       { return this->center.y - this->halfHeight; }
double AABB::getBottomY() const    { return this->center.y + this->halfHeight; }
double AABB::getLeftX() const      { return this->center.x - this->halfWidth; }
double AABB::getRightX() const     { return this->center.x + this->halfWidth; }

/* -- QuadTree -- */

// Constructors
QuadTree::QuadTree(AABB bounds) {
    this->bounds = bounds;
}

// Getters
AABB                QuadTree::getBounds() const    { return this->bounds; }
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

    if (fitsNorth) {
        if (fitsWest) {
            return 0;
        }
        if (fitsEast) {
            return 1;
        }
    }
    if (fitsSouth) {
        if (fitsWest) {
            return 2;
        }
        if (fitsEast) {
            return 3;
        }
    }

    return -1;
}

bool QuadTree::insert(GameObject* gobj) {
    return false;
}

vector<GameObject*> QuadTree::findCollidingObjects(AABB& box) const {
    return {};
}

vector<GameObject*> QuadTree::findCollidingObjects(AABB& box, vector<GameObject*> acc) const {
    return {};
}