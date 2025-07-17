#include "game.hpp"

#include <iostream>
#include <deque>

#include "objects.hpp"

using std::cin, std::cout, std::endl;
using std::deque;

int gameState = GS_LAUNCHED;
deque<GameObject*> gameObjects = {};

Player* player;

void doGame() {
	switch (gameState) {
		case GS_LAUNCHED:
			player = new Player();
			gameObjects.push_back(player);

			gameState = GS_STARTED;
			break;
		case GS_STARTED:
			break;
	}
}