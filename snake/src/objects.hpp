#ifndef OBJECTS_HPP
#define OBJECTS_HPP

// Apples that increase your length when eaten
class Apple {
	private:
		int x;
		int y;
	public:
		Apple(int x, int y);

		int getX() const;
		int getY() const;

		void setX(int x);
		void setY(int y);
};

#endif