// Game object class definitions and related logic

//TODO: make DIR_* consts into an enum?

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>

using std::string;

// Default values for a newly spawned player
const int PLR_WIDTH = 40;
const int PLR_HEIGHT = 80;
const double PLR_MOVESPEED = 2.5;

// Possible values for "direction" in GameObject
const int DIR_NONE = 0;
const int DIR_LEFT = 1;
const int DIR_RIGHT = 2;
const int DIR_UP = 3;
const int DIR_DOWN = 4;

// Abstract class for specialized objects to implement
class GameObject {
	protected:
		double x = 0;
		double y = 0;
		int width = 16;
		int height = 16;
		double speedX = 0;
		double speedY = 0;
		string state = "";
		int direction = DIR_NONE;
	public:
		double getX() const;
		double getY() const;
		int getWidth() const;
		int getHeight() const;
		double getSpeedX() const;
		double getSpeedY() const;
		string getState() const;
		int getDirection() const;

		void setWidth(int width);
		void setHeight(int height);
		void setSpeedX(double speedX);
		void setSpeedY(double speedY);
		void setState(string state);
		void setDirection(int direction);

		// Move object regardless of collision rules
		void teleport(double x, double y);

		// Move object while checking for collision at the target location
		bool tryMove(double x, double y);

		// Give the object X and Y speed
		void thrust(double addX, double addY);

		// Pure virtual destructor to ensure the class is abstract
		virtual ~GameObject() = 0;
};

// The player character
class Player : public GameObject {
	private:
		double moveSpeed = PLR_MOVESPEED;
	public:
		Player();
		Player(double x, double y);

		// Same as GameObject's, but only allowing DIR_LEFT and DIR_RIGHT
		void setDirection(int direction);

		// Make the player walk toward the direction they're facing
		bool walk();
};

#endif