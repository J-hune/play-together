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
 * @param player The player object.
 * @param obstacles Vector of polygons representing obstacles.
 */
void handleCollisionsWithObstacles(Player *player, const std::vector<Polygon> &obstacles);

/**
 * @brief Handles collisions between a player and 1D moving platforms.
 * @param player The player object.
 * @param platforms Vector of movingPlatform1D.
 * TODO: correct the collision when the platform move on y-axis
 */
void handleCollisionsWithMovingPlatform1D(Player *player, const std::vector<MovingPlatform1D> &platforms);

/**
 * @brief Handles collisions between a player and 2D moving platforms.
 * @param player The player object.
 * @param platforms Vector of movingPlatform2D.
 * TODO: correct the collision when the platform move on y-axis
 */
void handleCollisionsWithMovingPlatform2D(Player *player, const std::vector<MovingPlatform2D> &platforms);

/**
 * @brief Handles collisions between a player and switching platforms.
 * @param player The player object.
 * @param platforms Vector of switchingPlatform.
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
void handleCollisionsWithCameraBorders(Player *player, std::vector<Polygon> &deathZones);

/**
 * @brief Handles collisions between the player and death zones.
 * @param player The player object.
 * @param deathZones Vector of polygons representing death zones.
 */
bool handleCollisionsWithDeathZones(const Player &player, std::vector<Polygon> &deathZones);

/**
 * @brief Handles collisions between the player and save zones.
 * @param player The player object.
 * @param level Reference to the level object.
 * @param saveZones Vector of polygons representing save zones.
 */
void handleCollisionsWithSaveZones(const Player &player, Level &level, std::vector<Polygon> &savesZones);

/* PLAYER REVERSED MAVITY */

/**
 * @brief Handles collisions between a player and obstacles when mavity is reversed.
 * @param player The player object.
 * @param obstacles Vector of polygons representing the obstacles.
 */
void handleCollisionsSelcatsbOhtiw(Player *player, const std::vector<Polygon> &obstacles);

/**
 * @brief Handles collisions between a player and 1D moving platforms when mavity is reversed.
 * @param player The player object.
 * @param platforms Vector of movingPlatform1D.
 */
void handleCollisionsD1mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform1D> &platforms);

/**
 * @brief Handles collisions between a player and 2D moving platforms when mavity is reversed.
 * @param player The player object.
 * @param platforms Vector of movingPlatform2D.
 */
void handleCollisionsD2mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform2D> &platforms);

/**
 * @brief Handles collisions between a player and switching platforms when mavity is reversed.
 * @param player The player object.
 * @param platforms Vector of switchingPlatform.
 */
void handleCollisionsMroftalPgnihctiwShtiw(Player *player, const std::vector<SwitchingPlatform> &platforms);


/* COLLISIONS UNRELATED TO MAVITY */

/**
 * @brief Handles collisions between the player and items of generic type.
 * @tparam T The type of items to handle with.
 * @param player The player object.
 * @param level Reference to the level object.
 * @param items Vector of T representing the items.
 * @param type The type of items to retrieve.
 */
template<typename T>
void handleCollisionsWithItems(Player *player, Level *level, std::vector<T> &items, ItemTypes type) {
    // Check for collisions with each item
    for (T item : items) {
        // If a collision is detected, apply item's effect to the player
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            level->removeItem(item, type);
        }
    }
}

#endif //PLAY_TOGETHER_COLLISIONHANDLER_H
