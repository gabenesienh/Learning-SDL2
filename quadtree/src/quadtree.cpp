#include "quadtree.hpp"

#include <array>
#include <vector>

using std::array;
using std::vector;

Point::Point() {
    this->x = 0;
    this->y = 0;
}
Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

AABB::AABB() {
    this->center = Point(0, 0);
    this->inradius = 0;
}
AABB::AABB(Point center, double inradius) {
    this->center = center;
    this->inradius = inradius;
}
    
bool AABB::containsPoint(Point point) {
    if (point.x >= this->center.x - this->inradius
    &&  point.x <= this->center.x + this->inradius
    &&  point.y >= this->center.y - this->inradius
    &&  point.y <= this->center.y + this->inradius) {
        return true;
    }

    return false;
}

QuadTree::QuadTree() {
    this->boundary = AABB();
}
QuadTree::QuadTree(AABB boundary) {
    this->boundary = boundary;
}

array<QuadTree*, 4> QuadTree::getChildren() {
    return {nw, ne, sw, se};
}

void QuadTree::subdivide() {
    Point nwCenter = Point(
        this->boundary.center.x - this->boundary.inradius/2,
        this->boundary.center.y - this->boundary.inradius/2
    );
    Point neCenter = Point(
        this->boundary.center.x + this->boundary.inradius/2,
        this->boundary.center.y - this->boundary.inradius/2
    );
    Point swCenter = Point(
        this->boundary.center.x - this->boundary.inradius/2,
        this->boundary.center.y + this->boundary.inradius/2
    );
    Point seCenter = Point(
        this->boundary.center.x + this->boundary.inradius/2,
        this->boundary.center.y + this->boundary.inradius/2
    );

    this->nw = new QuadTree(AABB(nwCenter, this->boundary.inradius/2));
    this->ne = new QuadTree(AABB(neCenter, this->boundary.inradius/2));
    this->sw = new QuadTree(AABB(swCenter, this->boundary.inradius/2));
    this->se = new QuadTree(AABB(seCenter, this->boundary.inradius/2));

    // Move the points in this tree to the new subdivisions
    for (Point& point : this->points) {
        if (this->nw->insert(point)) continue;
        if (this->ne->insert(point)) continue;
        if (this->sw->insert(point)) continue;
        if (this->se->insert(point)) continue;
    }

    this->points = {};
}

bool QuadTree::insert(Point point) {
    // Don't insert if it's not in range
    if (!this->boundary.containsPoint(point)) {
        return false;
    }

    if (this->points.size() < 4 && this->nw == nullptr) {
        points.push_back(point);
        return true;
    }

    if (nw == nullptr) {
        this->subdivide();
    }

    if (this->nw->insert(point)) return true;
    if (this->ne->insert(point)) return true;
    if (this->sw->insert(point)) return true;
    if (this->se->insert(point)) return true;

    return false;
}