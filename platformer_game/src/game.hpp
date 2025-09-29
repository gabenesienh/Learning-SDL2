// Main game logic

#ifndef GAME_HPP
#define GAME_HPP

#include <deque>

#include "levels.hpp"
#include "objects.hpp"

using std::deque;

// Possible game states
const int GS_LAUNCHED = 0;
const int GS_STARTED = 1;

// Enables debug features
// 0x0001: Performance info in output
// 0x0010: Level info in output
// 0x0100: Player info in output
// 0x0100: Hitbox visualization
extern int debugMode;

// Delta time
// Should be measured in 60ths of a second for game physics to work correctly
extern double dt;

// The current game state, uses GS_* constants
extern int gameState;

// Points to currently loaded level
extern Level* loadedLevel;

// The objects currently present in the game
extern deque<GameObject*> gameObjects;

// Processes game logic for a frame
extern void doGame();

#endif