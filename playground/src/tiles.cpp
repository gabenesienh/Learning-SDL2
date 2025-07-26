#include "tiles.hpp"

/* -- TileType -- */

// Constructors
TileType::TileType(int width, int height) {
	this->width = width;
	this->height = height;
}

// Getters
int TileType::getWidth() const  { return this->width; }
int TileType::getHeight() const { return this->height; }

/* -- Tile -- */

int Tile::getX() const      { return this->x; }
int Tile::getY() const      { return this->y; }
int Tile::getTypeId() const { return this->typeId; }

// View the header file for info on constructor structures

const unordered_map<int, TileType> tileTypesTable = {
	{0, TileType(
		1,
		1
	)}
};