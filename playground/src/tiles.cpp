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

// Constructors
Tile::Tile(int typeId, int x, int y) {
	this->typeId = typeId;
	this->x = x;
	this->y = y;
}

// Getters
int Tile::getX() const      { return this->x; }
int Tile::getY() const      { return this->y; }
int Tile::getTypeId() const { return this->typeId; }

/* -- Other -- */

// View the header file for info on constructor structures

const unordered_map<int, TileType> tileTypesTable = {
	{1, TileType(
		32,
		32
	)}
};