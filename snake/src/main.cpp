// Simple snake game
// Move with arrow keys and collect apples

//TODO: score tracking
//TODO: options menu (apple count, speed)
//TODO: break program into multiple files

//TODO-OPT: high score tracking
//TODO-OPT: silly modes (wrap around, bombs)
//TODO-OPT: optimize cell checks by keeping track of empty spaces
//TODO-OPT: deglobalize variables

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <random>
#include <iterator>

#include "snake.hpp"
#include "events.hpp"
#include "objects.hpp"
#include "util.hpp"

using std::cin, std::cout, std::endl;
using std::string;
using std::array;
using std::vector;
using std::list;
using std::random_device, std::mt19937, std::uniform_int_distribution;
using std::next;

void doGame();
void doRender();

const int START_X = CELLS_HORIZONTAL/2;
const int START_Y = CELLS_VERTICAL/2;
const float SNAKE_MOVE_DELAY = 150; // In milliseconds
const float FLICKER_LOOP_DELAY = 1; // In seconds
const SDL_Color COLOR_BACKGROUND_RGB = {31, 31, 31, 255};
const SDL_Color COLOR_SNAKE_RGB = {255, 255, 255, 255};
const SDL_Color COLOR_APPLE_RGB = {255, 63, 63, 255};

const int GS_LAUNCHED = 0;
const int GS_PLAYING = 1;
const int GS_GAMEOVER = 2;

string checkCellContent(int checkX, int checkY);

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// For blitting everything to the screen, respecting the grid
SDL_Rect gameRendererRect = {0, 0, CELLS_SIZE, CELLS_SIZE};

// Global game state, defines what part of the game logic should run
int gameState = GS_LAUNCHED;

// Counts down to zero before allowing the snake to move
float snakeMoveTimer = SNAKE_MOVE_DELAY;

// Global object flickering timer, affected objects are invisible whenever the
// timer is on its higher half (i.e. higher than FLICKER_LOOP_DELAY/2)
float flickerTimer = 0;

// Random number generator
mt19937 rng(random_device{}());

// For generating random coordinates within the grid
uniform_int_distribution<int> distGridX(0, CELLS_HORIZONTAL - 1);
uniform_int_distribution<int> distGridY(0, CELLS_VERTICAL - 1);

// The snake the player controls
Snake snake = Snake();

// For storing all apples currently in game
vector<Apple> apples = {};

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (true) {
		deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;	
		ticksLast = SDL_GetTicks64();

		if (!doEvents()) break;
		doGame();
		doRender();
	}

	kill();
	return 0;
}

// Game logic
void doGame() {
	switch (gameState) {
		case GS_LAUNCHED:
			flickerTimer += 1*deltaTime;

			if (flickerTimer > FLICKER_LOOP_DELAY) {
				flickerTimer -= FLICKER_LOOP_DELAY;
			}

			if (keyStatesTap[SDL_SCANCODE_SPACE]
			||  keyStatesTap[SDL_SCANCODE_RETURN]) {
				gameState = GS_PLAYING;
				flickerTimer = 0;

				// Spawn apple
				apples.push_back(Apple(distGridX(rng), distGridY(rng)));
			}

			break;
		case GS_PLAYING:
		{
			if		(keyStatesTap[SDL_SCANCODE_UP])		{ snake.turn(SNAKE_DIR_UP); }
			else if	(keyStatesTap[SDL_SCANCODE_DOWN])	{ snake.turn(SNAKE_DIR_DOWN); }
			else if	(keyStatesTap[SDL_SCANCODE_LEFT])	{ snake.turn(SNAKE_DIR_LEFT); }
			else if	(keyStatesTap[SDL_SCANCODE_RIGHT])	{ snake.turn(SNAKE_DIR_RIGHT); }
		
			snakeMoveTimer -= 1000*deltaTime;

			if (snakeMoveTimer < 0) {
				// Attempt to move the snake
				if (!snake.move()) {
					gameState = GS_GAMEOVER;
					flickerTimer = FLICKER_LOOP_DELAY/2;
					break;
				}

				snakeMoveTimer += SNAKE_MOVE_DELAY;
			}

			// Detect if you're eating an apple after moving
			auto snakeHead = snake.getHead();

			for (auto& apple : apples) {
				if (snakeHead.getX() == apple.getX()
				&&  snakeHead.getY() == apple.getY()) {
					snake.grow();

					while (true) {
						// Pick a cell to teleport the apple to
						int checkX = distGridX(rng);
						int checkY = distGridY(rng);

						if (checkCellContent(checkX, checkY) == "empty") {
							// Teleport apple
							apple.setX(checkX);
							apple.setY(checkY);
							break;
						}
					}
				}
			}

			break;
		}
		case GS_GAMEOVER:
			flickerTimer += 1*deltaTime;

			if (flickerTimer > FLICKER_LOOP_DELAY) {
				flickerTimer -= FLICKER_LOOP_DELAY;
			}

			if (keyStatesTap[SDL_SCANCODE_SPACE]
			||  keyStatesTap[SDL_SCANCODE_RETURN]) {
				gameState = GS_LAUNCHED;
				flickerTimer = 0;

				snake.reset();
				apples.clear();
			}

			break;
	}

	// Reset tap inputs for the next frame
	keyStatesTap = {false};
}

// Clears screen and draws everything
void doRender() {
	Uint32 backgroundColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_BACKGROUND_RGB.r,
		COLOR_BACKGROUND_RGB.g,
		COLOR_BACKGROUND_RGB.b
	);

	Uint32 snakeColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_SNAKE_RGB.r,
		COLOR_SNAKE_RGB.g,
		COLOR_SNAKE_RGB.b
	);

	Uint32 appleColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_APPLE_RGB.r,
		COLOR_APPLE_RGB.g,
		COLOR_APPLE_RGB.b
	);

	// Clear screen before drawing
	SDL_FillRect(gameSurface, NULL, backgroundColor);

	// Draw apple(s)
	for (auto& apple : apples) {
		gameRendererRect.x = apple.getX()*CELLS_SIZE;
		gameRendererRect.y = apple.getY()*CELLS_SIZE;
		
		SDL_FillRect(gameSurface, &gameRendererRect, appleColor);
	}

	// Draw snake parts whenever the flicker timer isn't supposed to hide them
	if (flickerTimer <= FLICKER_LOOP_DELAY/2) {
		for (auto& segment : snake.getSegments()) {
			gameRendererRect.x = segment.getX()*CELLS_SIZE;
			gameRendererRect.y = segment.getY()*CELLS_SIZE;

			SDL_FillRect(gameSurface, &gameRendererRect, snakeColor);
		}
	}

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
}

// Returns what is currently occupying a given cell, or if it's empty
string checkCellContent(int checkX, int checkY) {
	// Check if the cell is a wall
	if (checkX < 0
	||  checkX > CELLS_HORIZONTAL - 1
	||  checkY < 0
	||  checkY > CELLS_VERTICAL - 1) {
		return "wall";
	}

	// Check for a snake segment in the cell
	for (auto& segment : snake.getSegments()) {
		if (checkX == segment.getX()
		&&  checkY == segment.getY()) {
			return "snake";
		}
	}

	// Check for an apple in the cell
	for (auto& apple : apples) {
		if (checkX == apple.getX()
		&&  checkY == apple.getY()) {
			return "apple";
		}
	}

	return "empty";
}