// Definitions for classes that are used for implementing a quadtree structure

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <array>
#include <vector>

#include "util.hpp"

using std::array;
using std::vector;

// Axis-aligned bounding box
class AABB {
    private:
        vec2   center;
        double width;
        double height;
    public:
        AABB(double width, double height);

        vec2   getCenter() const;
        double getWidth() const;
        double getHeight() const;
};

// Can either be the root node of a quadtree or a quadrant
class QuadTree {
    private:
        static const int BUCKET_CAPACITY = 4;

        AABB boundary;
        vector<vec2> points;
        QuadTree* nw = nullptr;
        QuadTree* ne = nullptr;
        QuadTree* sw = nullptr;
        QuadTree* se = nullptr;
    public:
        AABB         getBoundary() const;
        vector<vec2> getPoints() const;
        QuadTree*    getNW() const;
        QuadTree*    getNE() const;
        QuadTree*    getSW() const;
        QuadTree*    getSE() const;

        // Gets the NW, NE, SW and SE quadrants, in order
        array<QuadTree*, 4> getChildren() const;
};

#endif