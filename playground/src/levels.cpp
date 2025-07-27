#include "levels.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "tiles.hpp"

using std::string;
using std::unordered_map;
using std::vector;

/* -- Level -- */

// Constructors
Level::Level(string displayName, vector<Tile> tiles) {
	this->displayName = displayName;
	this->tiles = tiles;
}

// Getters
string       Level::getDisplayName() const { return this->displayName; }
vector<Tile> Level::getTiles() const       { return this->tiles; }

const unordered_map<string, Level> levelsTable = {
	{"test", Level(
		"Test",
		{
			Tile(
				1,
				256,
				320
			),
			Tile(
				1,
				288,
				320
			),
			Tile(
				1,
				320,
				320
			),
			Tile(
				1,
				256,
				352
			),
			Tile(
				1,
				320,
				352
			),
			Tile(
				1,
				256,
				384
			),
			Tile(
				1,
				288,
				384
			),
			Tile(
				1,
				320,
				384
			),
			Tile(
				1,
				256,
				416
			),
			Tile(
				1,
				256,
				448
			),
			Tile(
				1,
				384,
				320
			),
			Tile(
				1,
				416,
				320
			),
			Tile(
				1,
				448,
				320
			),
			Tile(
				1,
				384,
				352
			),
			Tile(
				1,
				448,
				352
			),
			Tile(
				1,
				384,
				384
			),
			Tile(
				1,
				448,
				384
			),
			Tile(
				1,
				384,
				416
			),
			Tile(
				1,
				448,
				416
			),
			Tile(
				1,
				384,
				448
			),
			Tile(
				1,
				416,
				448
			),
			Tile(
				1,
				448,
				448
			),
			Tile(
				1,
				384+32*4,
				320
			),
			Tile(
				1,
				416+32*4,
				320
			),
			Tile(
				1,
				448+32*4,
				320
			),
			Tile(
				1,
				384+32*4,
				352
			),
			Tile(
				1,
				448+32*4,
				352
			),
			Tile(
				1,
				384+32*4,
				384
			),
			Tile(
				1,
				448+32*4,
				384
			),
			Tile(
				1,
				384+32*4,
				416
			),
			Tile(
				1,
				448+32*4,
				416
			),
			Tile(
				1,
				384+32*4,
				448
			),
			Tile(
				1,
				416+32*4,
				448
			),
			Tile(
				1,
				448+32*4,
				448
			)
		}
	)}
};