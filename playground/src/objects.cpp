#include "objects.hpp"

#include <iostream>

const int PLR_WIDTH = 40;
const int PLR_HEIGHT = 80;

double GameObject::getX() const { return this->x; }
double GameObject::getY() const { return this->y; }
int GameObject::getWidth() const { return this->width; }
int GameObject::getHeight() const { return this->height; }

void GameObject::setWidth(int width) {
	this->width = width;
}
void GameObject::setHeight(int height) {
	this->height = height;
}

void GameObject::teleport(double x, double y) {
	this->x = x;
	this->y = y;
}

GameObject::~GameObject() {};

Player::Player() {
	this->width = PLR_WIDTH;
	this->height = PLR_HEIGHT;
}
Player::Player(double x, double y) {
	this->width = PLR_WIDTH;
	this->height = PLR_HEIGHT;
	this->x = x;
	this->y = y;
}

bool Player::walk(int direction) {
	switch (direction) {
		case DIR_LEFT:
			this->x -= this->moveSpeed;

			break;
		case DIR_RIGHT:
			this->x += this->moveSpeed;

			break;
	}

	return true;
}