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

// Flags for use with debugMode
const int DEBUG_PERFORMANCE_INFO = 0x0001;
const int DEBUG_LEVEL_INFO       = 0x0010;
const int DEBUG_PLAYER_INFO      = 0x0100;
const int DEBUG_SHOW_HITBOXES    = 0x1000;

// Enables debug features
// To be used with DEBUG_* consts, use bitwise operations to combine flags
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