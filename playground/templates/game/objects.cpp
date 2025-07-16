#include "objects.hpp"

#include <iostream>

int GameObject::getX() const { return this->x; }
int GameObject::getY() const { return this->y; }

GameObject::~GameObject() {};

/*
Player::Player() {}
Player::Player(int x, int y) {
	this->x = x;
	this->y = y;
}
*/