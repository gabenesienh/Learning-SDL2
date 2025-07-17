// Game object class definitions and related logic

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

// Abstract class for specialized objects to implement
class GameObject {
	protected:
		double x = 0;
		double y = 0;
		int width = 16;
		int height = 16;
	public:
		double getX() const;
		double getY() const;
		int getWidth() const;
		int getHeight() const;

		void setWidth(int width);
		void setHeight(int height);

		void teleport(double x, double y);

		virtual ~GameObject() = 0;
};

// Directions to send as parameters for some player methods
const int DIR_LEFT = 0;
const int DIR_RIGHT = 1;

// The player character
class Player : public GameObject {
	private:
		double moveSpeed = 2.5;
	public:
		Player();
		Player(double x, double y);

		bool walk(int direction);
};

#endif