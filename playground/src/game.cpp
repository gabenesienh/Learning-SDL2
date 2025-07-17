#include "game.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include <deque>

#include "events.hpp"
#include "objects.hpp"
#include "util.hpp"

using std::cin, std::cout, std::endl;
using std::deque;

bool debugMode = false;
int gameState = GS_LAUNCHED;

deque<GameObject*> gameObjects = {};

Player* player;

void doGame() {
	switch (gameState) {
		case GS_LAUNCHED:
			// Spawn player
			player = new Player();
			gameObjects.push_back(player);

			player->teleport(
				WINDOW_WIDTH/2,
				WINDOW_HEIGHT/2
			);

			gameState = GS_STARTED;
			break;
		case GS_STARTED:
			if (keyStates[SDL_SCANCODE_LEFT]) {
				player->walk(DIR_LEFT);
			}
			if (keyStates[SDL_SCANCODE_RIGHT]) {
				player->walk(DIR_RIGHT);
			}

			if (debugMode) {
				cout << player->getX() << ", " << player->getY() << '\n';
			}

			break;
	}
}