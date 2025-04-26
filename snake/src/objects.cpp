#include "objects.hpp"

Apple::Apple(int x, int y) {
	this->x = x;
	this->y = y;
}

int Apple::getX() const { return this->x; }
int Apple::getY() const { return this->y; }

void Apple::setX(int x) { this->x = x; }
void Apple::setY(int y) { this->y = y; }