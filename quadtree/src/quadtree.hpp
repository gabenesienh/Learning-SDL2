#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <array>
#include <vector>

using std::array;
using std::vector;

class Point {
    public:
        double x;
        double y;

        Point();
        Point(double x, double y);
};

class AABB {
    public:
        Point center;
        double inradius;
    
        AABB();
        AABB(Point center, double inradius);
    
        bool containsPoint(Point point);
};

class QuadTree {
    public:
        AABB boundary;
        vector<Point> points;
        QuadTree* nw = nullptr;
        QuadTree* ne = nullptr;
        QuadTree* sw = nullptr;
        QuadTree* se = nullptr;

        QuadTree();
        QuadTree(AABB boundary);

        array<QuadTree*, 4> getChildren();

        void subdivide();

        bool insert(Point point);
};

#endif