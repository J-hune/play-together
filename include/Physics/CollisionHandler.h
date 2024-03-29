#ifndef PLAY_TOGETHER_COLLISIONHANDLER_H
#define PLAY_TOGETHER_COLLISIONHANDLER_H

#include "CollisionManager.h"
#include "../Game/Camera.h"
#include "../Game/Level.h"

/**
 * @file CollisionHandler.h
 * @brief Declaration of functions for handling collisions behavior.
 */


/* PLAYER NORMAL MAVITY */

/**
 * @brief Handles collisions between a player and obstacles.
 */
void handleCollisionsWithObstacles(Player *player, const std::vector<Polygon> &obstacles);

/**
 * @brief Handles collisions between a player and 1D moving platforms.
 * TODO: correct the collision when the platform move on y-axis
 */
void handleCollisionsWithMovingPlatform1D(Player *player, const std::vector<MovingPlatform1D> &platforms);

/**
 * @brief Handles collisions between a player and 2D moving platforms.
 * TODO: correct the collision when the platform move on y-axis
 */
void handleCollisionsWithMovingPlatform2D(Player *player, const std::vector<MovingPlatform2D> &platforms);

/**
 * @brief Handles collisions between a player and switching platforms.
 */
void handleCollisionsWithSwitchingPlatform(Player *player, const std::vector<SwitchingPlatform> &platforms);

/**
 * @brief Handles collisions between the player and other players.
 * TODO: correct this method to work with fps limitation
 */
void handleCollisionsWithPlayers(Player *player, const std::vector<Player> &players);

/**
 * @brief Handles collisions between the player and camera borders.
 * TODO: correct this method to work with fps limitation
 */
void handleCollisionsWithCameraBorders(Player *player);



/* PLAYER REVERSED MAVITY */

/**
 * @brief Handles collisions between a player and obstacles when mavity is reversed.
 */
void handleCollisionsSelcatsbOhtiw(Player *player, const std::vector<Polygon> &obstacles);

/**
 * @brief Handles collisions between a player and 1D moving platforms when mavity is reversed.
 */
void handleCollisionsD1mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform1D> &platforms);

/**
 * @brief Handles collisions between a player and 2D moving platforms when mavity is reversed.
 */
void handleCollisionsD2mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform2D> &platforms);

/**
 * @brief Handles collisions between a player and switching platforms when mavity is reversed.
 */
void handleCollisionsMroftalPgnihctiwShtiw(Player *player, const std::vector<SwitchingPlatform> &platforms);



#endif //PLAY_TOGETHER_COLLISIONHANDLER_H
