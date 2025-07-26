#ifndef TILES_HPP
#define TILES_HPP

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

// For use with Tile class
// Defines common properties for all instances of Tile with the same TileType
class TileType {
	private:
		int width;
		int height;
	public:
		TileType(int width, int height);

		int getWidth() const;
		int getHeight() const;
};

// Represents a single tile in a map
class Tile {
	private:
		int typeId; //references tileTypesTable
		int x;
		int y;
	public:
		int getX() const;
		int getY() const;
		int getTypeId() const;
};

// All TileType definitions go here
// A TileType's key in this table will be its ID
extern const unordered_map<int, TileType> tileTypesTable;

#endif