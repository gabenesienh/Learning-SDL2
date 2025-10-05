//TODO: refactor GameObject::getScreen* methods into helper functions
//TODO: add weight attribute

// Game object class definitions and related logic

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>

#include "util.hpp"

using std::string;

// Default values for a newly spawned player
const int    PLR_WIDTH = 40;
const int    PLR_HEIGHT = 80;
const double PLR_MOVESPEED = 2.5;

// Direction values for GameObjects that move orthogonally
const vec2 DIR_NONE = {0, 0};
const vec2 DIR_LEFT = {-1, 0};
const vec2 DIR_RIGHT = {1, 0};
const vec2 DIR_UP = {0, -1};
const vec2 DIR_DOWN = {0, 1};

// Pre-defined sets of directions an object is allowed to have
enum class eDirTypes {
    none,
    horizontal,
    orthogonal,
    omni,
};

// Valid values for the position of a GameObject's alignment anchor
// The anchor is the point from which the object's X and Y values are
// calculated, from which the object will scale up or down in size, etc.
enum class eAnchorX {
    left,
    middle,
    right
};
enum class eAnchorY {
    top,
    middle,
    bottom
};

// Axis-aligned bounding box
struct AABB {
    vec2 center;
    double halfWidth;
    double halfHeight;

    AABB(vec2 center, double halfWidth, double halfHeight);

    // Gets the X or Y coordinate at the very edge of the specified side
    double getTopY() const;
    double getBottomY() const;
    double getLeftX() const;
    double getRightX() const;
};

// Abstract class for specialized objects to implement
class GameObject {
    protected:
        AABB      bounds        = AABB({0, 0}, 8, 8);
        eAnchorX  anchorOffsetX = eAnchorX::middle;
        eAnchorY  anchorOffsetY = eAnchorY::middle;
        double    speedX        = 0;
        double    speedY        = 0;
        double    moveSpeed     = 1;
        string    state         = "";
        vec2      direction     = DIR_NONE;
        eDirTypes directionType = eDirTypes::none;
    public:
        AABB&     getBounds();
        eAnchorX  getAnchorOffsetX() const;
        eAnchorY  getAnchorOffsetY() const;
        double    getSpeedX() const;
        double    getSpeedY() const;
        string    getState() const;
        vec2      getDirection() const;
        eDirTypes getDirectionType() const;

        // Gets the values from the object's bounding box, but adjusted for the
        // anchor alignment
        double getX() const;
        double getY() const;
        double getWidth() const;
        double getHeight() const;

        // Same as getX and getY, but adjusted for the camera's position
        double getScreenX() const;
        double getScreenY() const;

        void setWidth(double width);
        void setHeight(double height);
        void setSpeedX(double speedX);
        void setSpeedY(double speedY);
        void setState(string state);
        bool setDirection(vec2 direction);

        // Checks if the object is visible and should be rendered
        bool isVisible() const;

        // Move object regardless of collision rules
        void teleport(double x, double y);

        // Give the object X and Y speed
        void thrust(double addX, double addY);

        // Move object while checking for collision at the target location
        bool tryMove(double x, double y);

        // Move the object moveSpeed units toward the direction it's facing
        void walk();

        // Move the object moveSpeed units toward a direction
        void walk(vec2 direction);

        // Pure virtual destructor to ensure the class is abstract
        virtual ~GameObject() = 0;
};

// The player character
class Player : public GameObject {
    public:
        Player();
        Player(double x, double y);
};

#endif