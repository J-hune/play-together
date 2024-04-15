#ifndef PLAY_TOGETHER_PLAYERMANAGER_H
#define PLAY_TOGETHER_PLAYERMANAGER_H

#include "../Game.h"


/**
 * @file PlayerManager.h
 * @brief Defines the PlayerManager class responsible for handling players in the game.
 */


class PlayerManager {
private:
    /* ATTRIBUTES */

    Game *game;
    std::vector<Player> alivePlayers;
    std::vector<Player> deadPlayers;


public:
    /* CONSTRUCTOR */

    explicit PlayerManager(Game *game);


    /* ACCESSORS */

    /**
     * @brief Get the players in the game.
     * @return A vector of Player objects representing the players in the game.
     */
    [[nodiscard]] std::vector<Player> &getAlivePlayers();

    /**
     * @brief Get the dead players in the game.
     * @return A vector of Player objects representing the dead players in the game.
     */
    [[nodiscard]] std::vector<Player> &getDeadPlayers();

    [[nodiscard]] size_t getPlayerCount() const;


    /* METHODS */

    /**
     * @brief Get the player with the given id.
     * @param id The id of the player to find.
     * @return A pointer to the Player object with the given id.
     */
    Player *findPlayerById(int id);

    /**
     * @brief Get the index of the player with the given id.
     * @param id The id of the player to find.
     * @return The index of the player with the given id.
     */
    int findPlayerIndexById(int id);

    /**
     * @brief Get the average position of all players in the game.
     * @return A Point object representing the average position of all players in the game.
     */
    [[nodiscard]] Point getAveragePlayerPosition() const;

    /**
     * @brief Add a player to the game.
     * @param player The player to add.
     */
    void addPlayer(const Player &player);

    /**
     * @brief Remove a player from the game.
     * @param player A pointer to the player to remove.
     */
    void removePlayer(const Player &player);

    /**
     * @brief Kill a player in the game.
     * @param player The player to kill.
     */
    void killPlayer(Player &player);

    /**
     * @brief Respawn a player in the game.
     * @param player The player to respawn.
     */
    void respawnPlayer(Player &player);

    /**
     * @brief Clear all players from the game.
     */
    void clearPlayers();


};
#endif //PLAY_TOGETHER_PLAYERMANAGER_H
