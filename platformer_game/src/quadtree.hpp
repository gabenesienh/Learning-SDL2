// Definitions for classes that are used for implementing a quadtree structure

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <array>
#include <vector>

#include "objects.hpp"
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
        AABB();
        AABB(double width, double height);

        vec2   getCenter() const;
        double getWidth() const;
        double getHeight() const;
};

// Can either be the root node of a quadtree or a quadrant
class QuadTree {
    private:
        static const int BUCKET_CAPACITY = 4;

        AABB                bounds;
        vector<GameObject*> objects;
        QuadTree*           nw        = nullptr; // Northwest quadrant
        QuadTree*           ne        = nullptr; // Northeast quadrant
        QuadTree*           sw        = nullptr; // Southwest quadrant
        QuadTree*           se        = nullptr; // Southeast quadrant
    public:
        AABB                getBounds() const;
        vector<GameObject*> getObjects() const;
        QuadTree*           getNW() const;
        QuadTree*           getNE() const;
        QuadTree*           getSW() const;
        QuadTree*           getSE() const;

        // Gets the NW, NE, SW and SE quadrants, in order
        array<QuadTree*, 4> getChildren() const;

        QuadTree(AABB bounds);

        // Clears the objects of this node and clear its existing quadrants
        // recursively
        void clear();

        // Generate the NW, NE, SW and SE quadrants, "splitting" this node
        void subdivide();

        // Find which quadrant can fully hold the object
        // Returns -1 on error or if the object can't fit into any quadrant
        int findFittingQuadrant(GameObject* gobj);

        // Attempt to insert an object into this node
        bool insert(GameObject* gobj);

        // Recursively look and retrieve an object in this node
        GameObject* find();
};

#endif