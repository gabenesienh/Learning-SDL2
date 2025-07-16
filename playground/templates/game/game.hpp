// Main game logic

#ifndef GAME_HPP
#define GAME_HPP

// Possible game states
const int GS_LAUNCHED = 0;
const int GS_STARTED = 1;

// The current game state
extern int gameState;

// Processes game logic for a frame
extern void doGame();

#endif