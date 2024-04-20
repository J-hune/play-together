#include "../../../include/Game/GameManagers/PlayerManager.h"

/**
 * @file PlayerManager.cpp
 * @brief Implements the PlayerManager class responsible for handling players in the game.
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
    player.increaseDeathCount();
    removePlayer(player);
    deadPlayers.emplace_back(player);
}


void PlayerManager::setTheBestPlayer(){
    if(!alivePlayers.empty()){
        int max = 0;
        Player *playerMax = &alivePlayers[0];

        // Find player with the highest score
        for (Player &player: alivePlayers) {
            player.useDefaultTexture();
            if (max<=player.getScore()){
                max = player.getScore();
                playerMax = &player;
            }
        }

        // Update texture for the best player
        (*playerMax).useMedalTexture();
    }
}

void PlayerManager::respawnPlayer(Player &player) {
    // Remove the player from the dead players list
    if (int index = findPlayerIndexById(player.getPlayerID()); index != -1) {
        deadPlayers.erase(deadPlayers.begin() + index);
    }

    // Add the player to the alivePlayers vector
    alivePlayers.push_back(player);

    // Teleport the player to the last checkpoint
    Level const *level = game->getLevel();
    int last_checkpoint = level->getLastCheckpoint();
    size_t spawn_index = alivePlayers.size();
    Point spawn_point = level->getSpawnPoints(last_checkpoint)[spawn_index];
    player.setX(spawn_point.x);
    player.setY(spawn_point.y);
}

void PlayerManager::clearPlayers() {
    alivePlayers.clear();
    deadPlayers.clear();
}