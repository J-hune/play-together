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

Player *PlayerManager::findPlayerById(int id) {
    for (Player &player: alivePlayers) {
        if (player.getPlayerID() == id) {
            return &player;
        }
    }
    return nullptr;
}

int PlayerManager::findPlayerIndexById(int id) {
    for (std::vector<Player>::size_type i = 0; i < alivePlayers.size(); i++) {
        if (alivePlayers[i].getPlayerID() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Point PlayerManager::getAveragePlayerPosition() const {
    Point averagePosition = {0, 0};
    for (const Player &player: alivePlayers) {
        averagePosition.x += player.getX();
        averagePosition.y += player.getY();
    }

    if (!alivePlayers.empty()) {
        averagePosition.x /= static_cast<float>(alivePlayers.size());
        averagePosition.y /= static_cast<float>(alivePlayers.size());
    }

    return averagePosition;
}

void PlayerManager::addPlayer(const Player &player) {
    alivePlayers.emplace_back(player);
}

void PlayerManager::removePlayer(const Player &player) {
    alivePlayers.erase(std::remove(alivePlayers.begin(), alivePlayers.end(), player), alivePlayers.end());
    deadPlayers.erase(std::remove(deadPlayers.begin(), deadPlayers.end(), player), deadPlayers.end());
}


void PlayerManager::killPlayer(Player &player) {
    // Remove the player from the alive players list
    removePlayer(player);

    // Add the player to the dead players list
    deadPlayers.emplace_back(player);
}

void PlayerManager::respawnPlayer(Player &player) {
    // Remove the player from the dead players list
    if (int index = findPlayerIndexById(player.getPlayerID()); index != -1) {
        deadPlayers.erase(deadPlayers.begin() + index);
    }

    // Add the player to the alive players list
    alivePlayers.push_back(player);

    // Teleport the player to the last checkpoint
    Level const &level = game->getLevel();
    int lastCheckpoint = level.getLastCheckpoint();
    size_t spawnIndex = alivePlayers.size();
    Point spawnPoint = level.getSpawnPoints(lastCheckpoint)[spawnIndex];
    player.setX(spawnPoint.x);
    player.setY(spawnPoint.y);
}

void PlayerManager::clearPlayers() {
    alivePlayers.clear();
    deadPlayers.clear();
}