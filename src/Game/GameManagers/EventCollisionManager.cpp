#include "../../../include/Game/GameManagers/EventCollisionManager.h"

/**
 * @file EventCollisionManager.cpp
 * @brief Implements the EventCollisionManager class responsible for handling events collision behavior.
 */


/* CONSTRUCTORS */

EventCollisionManager::EventCollisionManager(Game *game) : gamePtr(game) {}


/* METHODS */

void EventCollisionManager::handleAsteroidsCollisions() {
    std::vector<Asteroid> asteroids = gamePtr->getLevel()->getAsteroids();
    const std::vector<Polygon>& collisionObstacles = gamePtr->getLevel()->getZones(PolygonType::COLLISION);

    for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end();) {
        Asteroid& asteroid = *asteroidIt;
        bool alreadyExplode = false;
        std::vector<Player>& characters = gamePtr->getPlayerManager().getAlivePlayers();

        // Check collisions with characters
        auto characterIt = characters.begin();
        while (!alreadyExplode && characterIt != characters.end()) {
            Player& character = *characterIt;
            if (checkAABBCollision(character.getBoundingBox(), asteroid.getBoundingBox())) {
                alreadyExplode = true;
                gamePtr->getPlayerManager().killPlayer(character);
            }
            ++characterIt;
        }

        // Check collisions with obstacles
        auto obstacleIt = collisionObstacles.begin();
        while (!alreadyExplode && obstacleIt != collisionObstacles.end()) {
            const Polygon& obstacle = *obstacleIt;
            if (checkSATCollision(asteroid.getVertices(), obstacle)) {
                alreadyExplode = true;
                gamePtr->getCamera()->setShake(250);
                break; // No need to check further obstacles if the asteroid already exploded
            }
            ++obstacleIt;
        }

        // Check if asteroid goes out of bounds
        if (!alreadyExplode && asteroid.getY() > gamePtr->getCamera()->getY() + gamePtr->getCamera()->getH() - asteroid.getH()) {
            alreadyExplode = true;
        }

        // Handle explosion or move to the next asteroid
        if (alreadyExplode) {
            asteroid.explode();
            asteroidIt = asteroids.erase(asteroidIt); // Remove the asteroid and update the iterator
        } else {
            ++asteroidIt; // Move to the next asteroid
        }
    }

    gamePtr->getLevel()->setAsteroids(asteroids);
}