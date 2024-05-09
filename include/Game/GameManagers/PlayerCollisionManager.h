#ifndef PLAY_TOGETHER_PLAYERCOLLISIONMANAGER_H
#define PLAY_TOGETHER_PLAYERCOLLISIONMANAGER_H

#include <queue>
#include "../../Physics/Collision.h"
#include "../Level.h"
#include "../Game.h"

/**
 * @file PlayerCollisionManager.h
 * @brief Defines the PlayerCollisionManager class responsible for handling player collision behavior.
 */


//data necessary to bring back to the old state the player
typedef struct {
    Item* item;
    time_t t;
} GameData;

class PlayerCollisionManager {
private:
    /* ATTRIBUTES */

    Game *gamePtr; /**< A pointer to the game object. */
    // Queue in order to keep track of the effects applied on the player
    std::queue<GameData*> timeQueue;



public:
    /* CONSTRUCTORS */

    explicit PlayerCollisionManager(Game *game);


    /* METHODS */

    /**
     * @brief Handles all collisions for all players in the game.
     * @param delta_time The time elapsed since the last frame.
     */
    void handleCollisions(double delta_time);



private:

    /* PLAYER NORMAL MAVITY */

    /**
     * @brief Handles collisions between a player and obstacles.
     * @param player The player object.
     */
    void handleCollisionsWithObstacles(Player *player);

    /**
     * @brief Handles collisions between a player and treadmill levers.
     * @param player The player object.
     * @return True if the player hits a treadmill lever, false otherwise.
     */
    bool handleCollisionsWithTreadmillLevers(const Player *player);

    /**
     * @brief Handles collisions between a player and platform levers.
     * @param player The player object.
     * @return True if the player hits a platform lever, false otherwise.
     */
    bool handleCollisionsWithPlatformLevers(const Player *player);

    /**
     * @brief Handles collisions between a player and crusher levers.
     * @param player The player object.
     * @return True if the player hits a crusher lever, false otherwise.
     */
    bool handleCollisionsWithCrusherLevers(const Player *player);

    /**
     * @brief Handles collisions between a player and 1D moving platforms.
     * @param player The player object.
     */
    void handleCollisionsWithMovingPlatform1D(Player *player);

    /**
     * @brief Handles collisions between a player and 2D moving platforms.
     * @param player The player object.
     */
    void handleCollisionsWithMovingPlatform2D(Player *player);

    /**
     * @brief Handles collisions between a player and switching platforms.
     * @param player The player object.
     */
    void handleCollisionsWithSwitchingPlatform(Player *player);

    /**
     * @brief Handles collisions between a player and weight platforms.
     * @param player The player object.
     */
    void handleCollisionsWithWeightPlatform(Player *player);

    /**
     * @brief Handles collisions between a player and treadmills.
     * @param player The player object.
     */
    void handleCollisionsWithTreadmills(Player *player);

    /**
     * @brief Handles collisions between the player and crushers.
     * @param player The player object.
     * @return True if the player dies, false otherwise.
     */
    bool handleCollisionsWithCrushers(Player *player);

    /**
     * @brief Handles collisions between the player and camera borders.
     * @return True if the player dies, false otherwise.
     */
    bool handleCollisionsWithCameraBorders(SDL_FRect player);

    /**
     * @brief Handles collisions between the player and death zones.
     * @param player The player object.
     * @return True if the player dies, false otherwise.
     */
    bool handleCollisionsWithDeathZones(const Player &player);

    /**
     * @brief Handles collisions between the player and save zones.
     * @param player The player object.
     */
    void handleCollisionsWithSaveZones(Player &player);

    /**
     * @brief Handles collisions between the player and rescue zones.
     * @param player The player object.
     */
    void handleCollisionsWithRescueZones(const Player &player);

    /**
     * @brief Handles collisions between the player and toggle gravity zones.
     * @param player The player object.
     * @param delta_time The time elapsed since the last frame.
     */
    void handleCollisionsWithToggleGravityZones(Player &player, double delta_time);

    /**
     * @brief Handles collisions between the player and increase fall speed zones.
     * @param player The player object.
     */
    void handleCollisionsWithIncreaseFallSpeedZones(Player &player);

    /**
     * @brief Handles collisions between the player and size power-up items.
     * @param player Reference to the player object.
     */
    void handleCollisionsWithSizePowerUps(Player *player);

    /**
     * @brief Handles collisions between the player and size power-up items.
     * @param player Reference to the player object.
     */
    void handleCollisionsWithSpeedPowerUps(Player *player);

    /**
     * @brief Handles collisions between the player and coins items.
     * @param player Reference to the player object.
     * @param level Reference to the level object.
     * @param coins Vector of Coin.
     * @param playerManager The manager responsible for handling player-related operations.
     */
    void handleCollisionsWithCoins(Player *player);

    /**
     * @brief Handles collisions between the player and items.
     * @param player The player object.
     */
    void handleCollisionsWithItem(Player *player);

    /**
     * @brief Handles collisions between the player and dead players.
     * @param player Reference to the player object.
     * @return True if the player hits a dead player, false otherwise.
     */
    bool handleCollisionsWithDeadPlayers(const Player *player);

};

#endif //PLAY_TOGETHER_PLAYERCOLLISIONMANAGER_H
