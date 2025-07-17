// Game object class definitions and related logic

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

// Abstract class for specialized objects to implement
class GameObject {
	protected:
		int x = 0;
		int y = 0;
		int width = 16;
		int height = 16;
	public:
		int getX() const;
		int getY() const;
		int getWidth() const;
		int getHeight() const;

		virtual ~GameObject() = 0;
};

// The player character
class Player : public GameObject {
	public:
		Player();
		Player(int x, int y);
};

#endif