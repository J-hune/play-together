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
 */
void handleCollisionsWithMovingPlatform1D(Player *player, const std::vector<MovingPlatform1D> &platforms);

/**
 * @brief Handles collisions between a player and 2D moving platforms.
 * @param player The player object.
 * @param platforms Vector of movingPlatform2D.
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
 * @return True if the player dies, false otherwise.
 */
bool handleCollisionsWithCameraBorders(SDL_FRect player, SDL_FRect camera);

/**
 * @brief Handles collisions between the player and death zones.
 * @param player The player object.
 * @param deathZones Vector of polygons representing death zones.
 * @return True if the player dies, false otherwise.
 */
bool handleCollisionsWithDeathZones(const Player &player, std::vector<Polygon> &deathZones);

/**
 * @brief Handles collisions between the player and save zones.
 * @param player The player object.
 * @param level Reference to the level object.
 * @param saveZones Vector of AABB representing save zones.
 */
void handleCollisionsWithSaveZones(Player &player, Level &level, std::vector<AABB> &savesZones);

/**
 * @brief Handles collisions between the player and toggle gravity zones.
 * @param player The player object.
 * @param toggleGravityZones Vector of AABB representing toggle gravity zones.
 */
void handleCollisionsWithToggleGravityZones(Player &player, const std::vector<AABB> &toggleGravityZones);

/**
 * @brief Handles collisions between the player and increase fall speed zones.
 * @param player The player object.
 * @param increaseFallSpeedZones Vector of AABB representing increase fall speed zones.
 */
void handleCollisionsWithIncreaseFallSpeedZones(Player &player, const std::vector<AABB> &increaseFallSpeedZones);


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
 * @brief Handles collisions between the player and size power-up items.
 * @param player Reference to the player object.
 * @param level Reference to the level object.
 * @param sizePowerUp Vector of SizePowerUp.
 * @param playerManager The manager responsible for handling player-related operations.
 */
void handleCollisionsWithSizePowerUp(Player *player, Level *level, std::vector<SizePowerUp> &sizePowerUp, PlayerManager &playerManager);

/**
 * @brief Handles collisions between the player and size power-up items.
 * @param player Reference to the player object.
 * @param level Reference to the level object.
 * @param speedPowerUp Vector of SizePowerUp.
 * @param playerManager The manager responsible for handling player-related operations.
 */
void handleCollisionsWithSpeedPowerUp(Player *player, Level *level, std::vector<SpeedPowerUp> &speedPowerUp, PlayerManager &playerManager);


#endif //PLAY_TOGETHER_COLLISIONHANDLER_H
