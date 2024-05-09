#ifndef PLAY_TOGETHER_EVENTCOLLISIONMANAGER_H
#define PLAY_TOGETHER_EVENTCOLLISIONMANAGER_H

#include "../Game.h"

/**
 * @file EventCollisionManager.h
 * @brief Defines the EventCollisionManager class responsible for handling events collision behavior.
 */

class EventCollisionManager {
private:
    /* ATTRIBUTES */

    Game *gamePtr; /**< A pointer to the game object. */


public:
    /* CONSTRUCTORS */

    explicit EventCollisionManager(Game *game);


    /* METHODS */

    /**
     * @brief Handles collisions between the asteroid and every collisions.
     */
    void handleAsteroidsCollisions();

};


#endif //PLAY_TOGETHER_EVENTCOLLISIONMANAGER_H
