//TODO: validate position on GameObject::tryMove

#include "objects.hpp"

#include <string>

using std::string;

/* -- GameObject -- */

// Getters
double GameObject::getX() const         { return this->x; }
double GameObject::getY() const         { return this->y; }
int    GameObject::getWidth() const     { return this->width; }
int    GameObject::getHeight() const    { return this->height; }
double GameObject::getSpeedX() const    { return this->speedX; }
double GameObject::getSpeedY() const    { return this->speedY; }
string GameObject::getState() const     { return this->state; }
int    GameObject::getDirection() const { return this->direction; }

// Setters
void GameObject::setWidth(int width)         { this->width = width; }
void GameObject::setHeight(int height)       { this->height = height; }
void GameObject::setSpeedX(double speedX)    { this->speedX = speedX; }
void GameObject::setSpeedY(double speedY)    { this->speedY = speedY; }
void GameObject::setState(string state)      { this->state = state; }
void GameObject::setDirection(int direction) { this->direction = direction; }

// Other methods
void GameObject::teleport(double x, double y) {
	this->x = x;
	this->y = y;
}
bool GameObject::tryMove(double x, double y) {
	this->teleport(x, y);
	return true;
}
void GameObject::thrust(double addX, double addY) {
	this->speedX += addX;
	this->speedY += addY;
}

GameObject::~GameObject() {};

/* -- Player -- */

// Constructors
Player::Player() {
	this->width = PLR_WIDTH;
	this->height = PLR_HEIGHT;
	this->state = "stand";
	this->direction = DIR_RIGHT;
}
Player::Player(double x, double y) : Player() {
	this->x = x;
	this->y = y;
}

// Setters
void Player::setDirection(int direction) {
	switch (direction) {
		case DIR_LEFT:
		case DIR_RIGHT:
			this->direction = direction;
			break;
		default:
			this->direction = DIR_RIGHT;
			break;
	}
}

// Other methods
bool Player::walk() {
	this->state = "walk";

	switch (this->direction) {
		case DIR_LEFT:
			this->setSpeedX(-this->moveSpeed);
			break;
		case DIR_RIGHT:
			this->setSpeedX(this->moveSpeed);
			break;
	}

	return true;
}