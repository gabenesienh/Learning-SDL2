//TODO: proper collision detection

#include "game.hpp"

#include <SDL2/SDL.h>
#include <cmath>
#include <deque>
#include <iostream>
#include <string>

#include "events.hpp"
#include "levels.hpp"
#include "objects.hpp"
#include "preferences.hpp"
#include "util.hpp"

using std::min;
using std::deque;
using std::cin, std::cout, std::endl;
using std::string;

const double GRAV_ADD = 0.15;
const double GRAV_MULT = 1.05;
const double GRAV_CAP = 10;

// Loads the specified level from levelsTable
Level* loadLevel(string levelName);

bool debugMode = false;
int gameState = GS_LAUNCHED;

// Points to a copy of a level from levelsTable
// Receives a value upon calling loadLevel
Level* loadedLevel;

deque<GameObject*> gameObjects = {};

Player* player; //the player object in gameObjects

void doGame() {
    switch (gameState) {
        case GS_LAUNCHED:
            // Load level
            loadedLevel = loadLevel("test");

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
            if (mouseScreenPos.x < player->getScreenX()) {
                player->setDirection(DIR_LEFT);
            } else if (mouseScreenPos.x > player->getScreenX()) {
                player->setDirection(DIR_RIGHT);
            }

            /* -- Physics -- */

            for (auto gobj : gameObjects) {
                // Apply gravity to objects
                gobj->setSpeedY(gobj->getSpeedY()*GRAV_MULT);
                gobj->thrust(0, GRAV_ADD);

                // Cap falling speed
                gobj->setSpeedY(min(gobj->getSpeedY(), GRAV_CAP));

                // Displace game objects based on their speed
                double targetX = gobj->getX() + gobj->getSpeedX();
                double targetY = gobj->getY() + gobj->getSpeedY();

                gobj->tryMove(targetX, targetY);

                // TEMPORARY INVISIBLE GROUND
                if (gobj->getY() > 320) {
                    gobj->teleport(gobj->getX(), 320);
                }
            }

            /* -- Debug -- */

            // Show debug info if enabled
            if (debugMode) {
                cout <<  "Level " << '\n';
                cout << " name=" << loadedLevel->getDisplayName() << '\n';
                cout << '\n';
                cout <<  "Player" << '\n';
                cout << " x=" << player->getX()
                     << " y=" << player->getY() << '\n';
                cout << " spdx=" << player->getSpeedX()
                     << " spdy=" << player->getSpeedY()
                     << std::showpoint << '\n';
                cout << " dirx=" << player->getDirection().x
                     << " diry=" << player->getDirection().y
                     << std::showpoint << '\n';
            }

            break;
    }
}

Level* loadLevel(string levelName) {
    try {
        Level levelToCopy = levelsTable.at(levelName);

        Level* copiedLevel = new Level(
            levelToCopy.getDisplayName(),
            levelToCopy.getTiles()
        );

        return copiedLevel;
    } catch (std::out_of_range e) {
        if (debugMode) {
            cout << "ERROR: Attempted to load level that doesn't exist" << '\n';
        }

        return nullptr;
    }
}