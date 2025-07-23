#include "game.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include <deque>

#include "events.hpp"
#include "objects.hpp"
#include "preferences.hpp"
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
			if (keyStates[BT_LEFT]
			&& !keyStates[BT_RIGHT]) {
				player->setState("walk");
				player->walk(DIR_LEFT);
			} else if (keyStates[BT_RIGHT]
			       && !keyStates[BT_LEFT]) {
				player->setState("walk");
				player->walk(DIR_RIGHT);
			} else if (player->getState() == "walk") {
				player->setSpeedX(0);
				player->setState("stand");
			}

			// Have the player face the cursor
			if (mousePos.x < player->getX()) {
				player->setDirection(DIR_LEFT);
			} else if (mousePos.x > player->getX()) {
				player->setDirection(DIR_RIGHT);
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
				     << " dirx=" << player->getDirection().x
				     << " diry=" << player->getDirection().y
				     << std::showpoint << '\n';
			}

			break;
	}
}