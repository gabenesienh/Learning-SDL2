// Game object class definitions and related logic

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>

#include "util.hpp"

using std::string;

// Default values for a newly spawned player
const int PLR_WIDTH = 40;
const int PLR_HEIGHT = 80;
const double PLR_MOVESPEED = 2.5;

// Direction values for GameObjects that move orthogonally
const vec2 DIR_NONE = {0, 0};
const vec2 DIR_LEFT = {-1, 0};
const vec2 DIR_RIGHT = {1, 0};
const vec2 DIR_UP = {0, -1};
const vec2 DIR_DOWN = {0, 1};

// Pre-defined sets of directions an object is allowed to have
// Used in GameObject->setDirection() to prevent invalid directions for objects
enum eDirTypes {
	none,
	horizontal,
	orthogonal,
	omni,
};

// Abstract class for specialized objects to implement
class GameObject {
	protected:
		double    x             = 0;
		double    y             = 0;
		int       width         = 16;
		int       height        = 16;
		double    speedX        = 0;
		double    speedY        = 0;
		double    moveSpeed     = 1;
		string    state         = "";
		vec2      direction     = DIR_NONE;
		eDirTypes directionType = eDirTypes::none;
	public:
		double    getX() const;
		double    getY() const;
		int       getWidth() const;
		int       getHeight() const;
		double    getSpeedX() const;
		double    getSpeedY() const;
		string    getState() const;
		vec2      getDirection() const;
		eDirTypes getDirectionType() const;

		void setWidth(int width);
		void setHeight(int height);
		void setSpeedX(double speedX);
		void setSpeedY(double speedY);
		void setState(string state);
		bool setDirection(vec2 direction);

		// Move object regardless of collision rules
		void teleport(double x, double y);

		// Move object while checking for collision at the target location
		bool tryMove(double x, double y);

		// Give the object X and Y speed
		void thrust(double addX, double addY);

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