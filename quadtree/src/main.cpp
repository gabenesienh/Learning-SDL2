// Quadtree implementation test

#include <SDL2/SDL.h>

#include "events.hpp"
#include "util.hpp"

class Node {
    public:
        virtual ~Node() = 0;
};

class Point : public Node {
    public:
        int x;
        int y;

        Point() {
            this->x = 0;
            this->y = 0;
        }

        Point(int x, int y) {
            this->x = x;
            this->y = y;
        }
};

class AABB {
    public:
        Point center;
        double inradius;
    
        AABB(Point center, double inradius) {
            this->center = center;
            this->inradius = inradius;
        }
    
        bool containsPoint(Point point) {
            if (point.x >= this->center.x - this->inradius
            &&  point.x <= this->center.x + this->inradius
            &&  point.y >= this->center.y - this->inradius
            &&  point.y <= this->center.y + this->inradius) {
                return true;
            }

            return false;
        }
};

class Subdivision {
    public:
        AABB boundary;

        Node* nw;
        Node* ne;
        Node* sw;
        Node* se;
};

int main(int argc, char** argv) {
    if (!init()) return 1;

    while (true) {
        if (!doEvents()) break;
    }

    kill();
    return 0;
}