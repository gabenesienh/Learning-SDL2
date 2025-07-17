// Main game logic

#ifndef GAME_HPP
#define GAME_HPP

#include <deque>

#include "objects.hpp"

using std::deque;

// Possible game states
const int GS_LAUNCHED = 0;
const int GS_STARTED = 1;

// Enables debug features (e.g. hitboxes)
extern bool debugMode;

// The current game state
extern int gameState;

// The objects currently present in the game
extern deque<GameObject*> gameObjects;

// Processes game logic for a frame
extern void doGame();

#endif