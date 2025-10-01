#include "quadtree.hpp"

#include <array>

using std::array;

/* -- AABB -- */

// Constructors
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

// Getters
AABB         QuadTree::getBoundary() const { return this->boundary; }
vector<vec2> QuadTree::getPoints() const   { return this->points; }
QuadTree*    QuadTree::getNW() const       { return this->nw; }
QuadTree*    QuadTree::getNE() const       { return this->ne; }
QuadTree*    QuadTree::getSW() const       { return this->sw; }
QuadTree*    QuadTree::getSE() const       { return this->se; }

array<QuadTree*, 4> QuadTree::getChildren() const {
    return {this->nw, this->ne, this->sw, this->se};
}

// Setters

// Other methods