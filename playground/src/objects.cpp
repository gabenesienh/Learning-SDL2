//TODO: validate position on GameObject::tryMove
//TODO: validate setDirection based on directionType

#include "objects.hpp"

#include <string>
#include <cmath>

#include "util.hpp"

using std::string;
using std::abs;

/* -- GameObject -- */

// Getters
double      GameObject::getX() const             { return this->x; }
double      GameObject::getY() const             { return this->y; }
int         GameObject::getWidth() const         { return this->width; }
int         GameObject::getHeight() const        { return this->height; }
double      GameObject::getSpeedX() const        { return this->speedX; }
double      GameObject::getSpeedY() const        { return this->speedY; }
string      GameObject::getState() const         { return this->state; }
vec2        GameObject::getDirection() const     { return this->direction; }
eDirTypes   GameObject::getDirectionType() const { return this->directionType; }

// Setters
void GameObject::setWidth(int width)         { this->width = width; }
void GameObject::setHeight(int height)       { this->height = height; }
void GameObject::setSpeedX(double speedX)    { this->speedX = speedX; }
void GameObject::setSpeedY(double speedY)    { this->speedY = speedY; }
void GameObject::setState(string state)      { this->state = state; }
bool GameObject::setDirection(vec2 direction) {
	// Normalize desired direction into unit vector
	direction.normalize();

	// Ensure that the desired direction matches the object's direction type
	switch (this->directionType) {
		case eDirTypes::none:
			if (direction != DIR_NONE) {
				return false;
			}
			break;
		case eDirTypes::horizontal:
			if (direction != DIR_LEFT
			&&  direction != DIR_RIGHT) {
				return false;
			}
			break;
		case eDirTypes::orthogonal:
			if (direction != DIR_LEFT
			&&  direction != DIR_RIGHT
			&&  direction != DIR_UP
			&&  direction != DIR_DOWN) {
				return false;
			}
			break;
		case eDirTypes::omni:
			break;
	}

	this->direction = direction;
	return true;
}

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
void GameObject::walk() {
	this->speedX = this->direction.x * this->moveSpeed;
	this->speedY = this->direction.y * this->moveSpeed;
}

GameObject::~GameObject() {};

/* -- Player -- */

// Constructors
Player::Player() {
	this->width = PLR_WIDTH;
	this->height = PLR_HEIGHT;
	this->moveSpeed = PLR_MOVESPEED;
	this->state = "stand";
	this->direction = DIR_RIGHT;
	this->directionType = eDirTypes::horizontal;
}
Player::Player(double x, double y) : Player() {
	this->x = x;
	this->y = y;
}