#include "quadtree.hpp"

#include <array>

#include "objects.hpp"

using std::array;

/* -- AABB -- */

// Constructors
AABB::AABB() {
    this->width = 0;
    this->height = 0;
}
AABB::AABB(double width, double height) {
    this->width = width;
    this->height = height;
}

// Getters
vec2   AABB::getCenter() const { return this->center; }
double AABB::getWidth() const  { return this->width; }
double AABB::getHeight() const { return this->height; }

/* -- QuadTree -- */

// Constructors
QuadTree::QuadTree(AABB bounds) {
    this->bounds = bounds;
}

// Getters
AABB                QuadTree::getBounds() const  { return this->bounds; }
vector<GameObject*> QuadTree::getObjects() const   { return this->objects; }
QuadTree*           QuadTree::getNW() const        { return this->nw; }
QuadTree*           QuadTree::getNE() const        { return this->ne; }
QuadTree*           QuadTree::getSW() const        { return this->sw; }
QuadTree*           QuadTree::getSE() const        { return this->se; }

array<QuadTree*, 4> QuadTree::getChildren() const {
    return {this->nw, this->ne, this->sw, this->se};
}

// Other methods
void QuadTree::clear() {
    // Delete all the objects this node holds
    for (auto gobj : this->objects) {
        delete(gobj);
    }

    this->objects.clear();

    // Recursively delete all objects contained in this node's quadrants
    // Also delete the quadrants themselves
    if (this->nw != NULL) {
        for (auto node : this->getChildren()) {
            node->clear();
            delete(node);
            node = nullptr;
        }
    }
}

void QuadTree::subdivide() {

}

int QuadTree::findFittingQuadrant(GameObject* gobj) {
    return -1;
}

bool QuadTree::insert(GameObject* gobj) {
    return false;
}

GameObject* QuadTree::find() {
    return nullptr;
}