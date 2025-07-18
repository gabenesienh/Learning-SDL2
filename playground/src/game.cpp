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
			player = new Player(
				WINDOW_WIDTH/2,
				WINDOW_HEIGHT/2
			);
			gameObjects.push_back(player);

			gameState = GS_STARTED;
			break;
		case GS_STARTED:
			/* -- Player input handling -- */

			// Walking
			if (keyStates[SDL_SCANCODE_LEFT]
			&& !keyStates[SDL_SCANCODE_RIGHT]) {
				player->setDirection(DIR_LEFT);
				player->walk();
			} else if (keyStates[SDL_SCANCODE_RIGHT]
			       && !keyStates[SDL_SCANCODE_LEFT]) {
				player->setDirection(DIR_RIGHT);
				player->walk();
			} else if (player->getState() == "walk") {
				player->setSpeedX(0);
				player->setState("stand");
			}

			/* -- Physics -- */

			// Displace game objects based on their speed
			for (auto gobj : gameObjects) {
				double targetX = gobj->getX() + gobj->getSpeedX();
				double targetY = gobj->getY() + gobj->getSpeedY();

				gobj->tryMove(targetX, targetY);
			}

			/* -- Debug -- */

			// Show debug info if enabled
			if (debugMode) {
				cout <<  "x=" << player->getX()
				     << " y=" << player->getY()
				     << " dir=" << player->getDirection()
				     << '\n';
			}

			break;
	}
}