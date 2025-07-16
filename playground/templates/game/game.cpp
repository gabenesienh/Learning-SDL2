#include "game.hpp"

#include "objects.hpp"

int gameState = GS_LAUNCHED;

void doGame() {
	switch (gameState) {
		case GS_LAUNCHED:
			gameState = GS_STARTED;
			break;
		case GS_STARTED:
			break;
	}
}