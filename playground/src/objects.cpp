#include "objects.hpp"

#include <iostream>

int GameObject::getX() const { return this->x; }
int GameObject::getY() const { return this->y; }
int GameObject::getWidth() const { return this->width; }
int GameObject::getHeight() const { return this->height; }

GameObject::~GameObject() {};

Player::Player() {}
Player::Player(int x, int y) {
	this->x = x;
	this->y = y;
}