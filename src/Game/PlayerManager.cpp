#include "../../include/Game/PlayerManager.h"

/**
 * @file PlayerManager.cpp
 * @brief Implements the PlayerManager class responsible for handling the players in the game.
 */

/* CONSTRUCTOR */

PlayerManager::PlayerManager(Game *game) : game(game) {}


/* ACCESSORS */

std::vector<Player> &PlayerManager::getAlivePlayers() {
    return alivePlayers;
}

std::vector<Player> &PlayerManager::getDeadPlayers() {
    return deadPlayers;
}

size_t PlayerManager::getPlayerCount() const {
    return alivePlayers.size() + deadPlayers.size();
}

/* METHODS */

Player* PlayerManager::findPlayerById(int id) {
    auto it = std::ranges::find_if(alivePlayers, [id](const Player& player) {
        return player.getPlayerID() == id;
    });
    return (it != alivePlayers.end()) ? std::to_address(it) : nullptr;
}

int PlayerManager::findPlayerIndexById(int id) {
    auto it = std::ranges::find_if(alivePlayers, [id](const Player& player) {
        return player.getPlayerID() == id;
    });
    return (it != alivePlayers.end()) ? static_cast<int>(std::distance(alivePlayers.begin(), it)) : -1;
}

Point PlayerManager::getAveragePlayerPosition() const {
    Point average_position = {0, 0};
    for (const Player &player: alivePlayers) {
        average_position.x += player.getX();
        average_position.y += player.getY();
    }

    if (!alivePlayers.empty()) {
        average_position.x /= static_cast<float>(alivePlayers.size());
        average_position.y /= static_cast<float>(alivePlayers.size());
    }

    return average_position;
}

void PlayerManager::addPlayer(const Player &player) {
    alivePlayers.push_back(player);
}

void PlayerManager::removePlayer(const Player &player) {
    std::erase(alivePlayers, player);
    std::erase(deadPlayers, player);
}


void PlayerManager::killPlayer(Player &player) {
    removePlayer(player);
    deadPlayers.emplace_back(player);
    std::cout << "_______________________________" << std::endl;
    //For each player, print the player's ID and score
    std::cout << "APPEL DE LA METHODE KILL PLAYER" << std::endl;
    for (Player const &deadPlayer: deadPlayers) {
        std::cout << "Player ID: " << deadPlayer.getPlayerID() << std::endl;
    }
    std::cout << "_______________________________" << std::endl;
}

void PlayerManager::respawnPlayer(Player &player) {
    // Remove the player from the dead players list
    if (int index = findPlayerIndexById(player.getPlayerID()); index != -1) {
        deadPlayers.erase(deadPlayers.begin() + index);
    }

    //
    auto it = std::find(deadPlayers.begin(), deadPlayers.end(),player);
    if (it != deadPlayers.end()) {
        deadPlayers.erase(it);
    }

    // Add the player to the alive players list
    alivePlayers.push_back(player);

    // Teleport the player to the last checkpoint
    Level const &level = game->getLevel();
    int last_checkpoint = level.getLastCheckpoint();
    size_t spawn_index = alivePlayers.size();
    Point spawn_point = level.getSpawnPoints(last_checkpoint)[spawn_index];
    player.setX(spawn_point.x);
    player.setY(spawn_point.y);
}

void PlayerManager::clearPlayers() {
    alivePlayers.clear();
    deadPlayers.clear();
}