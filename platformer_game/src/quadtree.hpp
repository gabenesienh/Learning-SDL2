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
    friend class QuadTree;

    private:
        vec2   center;
        double halfWidth;
        double halfHeight;
    public:
        AABB();
        AABB(vec2 center, double halfWidth, double halfHeight);

        vec2   getCenter() const;
        double getHalfWidth() const;
        double getHalfHeight() const;

        // Gets the X or Y coordinate at the very edge of the specified side
        double getTopY() const;
        double getBottomY() const;
        double getLeftX() const;
        double getRightX() const;
};

// A quadtree for holding GameObjects
// Can either be the root node of a quadtree or a quadrant
class QuadTree {
    private:
        static const int BUCKET_CAPACITY = 4;

        AABB                bounds;
        vector<GameObject*> objects;
        array<QuadTree*, 4> quads    = {nullptr}; // NW, NE, SW and SE quadrants

        // Same as the public version, but with an accumulator for recursion
        // See the public version of this method for details
        vector<GameObject*> findCollidingObjects(AABB& box, vector<GameObject*> acc) const;
    public:
        QuadTree(AABB bounds);

        AABB                getBounds() const;
        vector<GameObject*> getObjects() const;
        array<QuadTree*, 4> getQuadrants() const;

        // Clears the objects of this node and clear its existing quadrants
        // recursively
        void clear();

        // Generate the NW, NE, SW and SE quadrants, "splitting" this node
        void subdivide();

        // Find the index of whichever quadrant could hold this bounding box
        // Returns -1 on error or if the box can't fully fit into any quadrant
        // NOTE: will *not* check if the quadrants actually exist! (i.e. if this
        // node has been subdivided)
        int findFittingQuadrant(AABB& box) const;

        // Attempt to insert an object into this node
        // If necessary, this node will be subdivided and all its objects will
        // try to fit into a quadrant
        bool insert(GameObject* gobj);

        // Recursively look for objects whose AABB intersect the given box's
        // Returns a list of matched objects
        vector<GameObject*> findCollidingObjects(AABB& box) const;
};

#endif