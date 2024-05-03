#include "../../../include/Game/GameManagers/PlayerManager.h"

/**
 * @file PlayerManager.cpp
 * @brief Implements the PlayerManager class responsible for handling players in the game.
 */


/* CONSTRUCTORS */

PlayerManager::PlayerManager(Game *game) : game(game) {}


/* ACCESSORS */

std::vector<Player> &PlayerManager::getAlivePlayers() {
    return alivePlayers;
}

std::vector<Player> &PlayerManager::getNeutralPlayers() {
    return neutralPlayers;
}

std::vector<Player> &PlayerManager::getDeadPlayers() {
    return deadPlayers;
}

size_t PlayerManager::getPlayerCount() const {
    return alivePlayers.size() + deadPlayers.size();
}


/* MUTATORS */

void PlayerManager:: setCurrentRescueZone(AABB zone) {
    // Change the rescue zone if it is different from the current one
    if (currentRescueZone.getID() != zone.getID()) {
        currentRescueZone.setZone(zone);

        // Teleport all dead players to the new rescue zone
        for (Player &player: deadPlayers) {
            player.teleport(currentRescueZone.getNextPosition(), game->getCamera()->getY());
        }
    }
}


/* METHODS */

Player* PlayerManager::findPlayerById(int id) {
    auto it1 = std::ranges::find_if(alivePlayers, [id](const Player& player) {
        return player.getPlayerID() == id;
    });

    auto it2 = std::ranges::find_if(deadPlayers, [id](const Player& player) {
        return player.getPlayerID() == id;
    });

    return (it1 != alivePlayers.end()) ? std::to_address(it1) : (it1 != deadPlayers.end()) ? std::to_address(it2) : nullptr;
}

int PlayerManager::findPlayerIndexById(int id) {
    auto it = std::ranges::find_if(alivePlayers, [id](const Player& player) {
        return player.getPlayerID() == id;
    });
    return (it != alivePlayers.end()) ? static_cast<int>(std::distance(alivePlayers.begin(), it)) : -1;
}

Point PlayerManager::getAveragePlayerPosition() const {
    Point average_position = {0, 0};

    // Calculate the average position of living players ...
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
    std::erase(neutralPlayers, player);
    std::erase(deadPlayers, player);
}


void PlayerManager::killPlayer(Player &player) {
    player.setMoveY(0);
    player.teleport(currentRescueZone.getNextPosition(), game->getCamera()->getY());
    /*
    // Reset keyboard state
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        if (keyboardState[i]) {
            SDL_KeyboardEvent keyEvent;
            keyEvent.keysym.scancode = static_cast<SDL_Scancode>(i);
            game->getInputManager().handleKeyUpEvent(findPlayerById(player.getPlayerID()), keyEvent);
        }
    }

    // Set player attributes
    player.setMoveY(0);
    player.setJumpMaxHeight(16);
    player.increaseDeathCount();
    player.setDeathAnimation();
    player.setIsAlive(false);

    // Kill player
    std::erase(alivePlayers, player);
    neutralPlayers.push_back(player);
     */
}

void PlayerManager::respawnPlayer(Player &player) {
    // Reset player attributes
    player.setJumpMaxHeight(100);
    player.setRespawnAnimation();
    player.setIsAlive(true);

    // Respawn player
    std::erase(deadPlayers, player);
    neutralPlayers.push_back(player);
}

void PlayerManager::moveNeutralPlayer(Player &player, int state) {
    if (state < 0) {
        // Move player to dead players list
        player.teleport(currentRescueZone.getNextPosition(), game->getCamera()->getY());
        std::erase(neutralPlayers, player);
        deadPlayers.push_back(player);
    } else {
        // Move player to alive players list
        std::erase(neutralPlayers, player);
        alivePlayers.push_back(player);
    }
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

void PlayerManager::clearPlayers() {
    alivePlayers.clear();
    neutralPlayers.clear();
    deadPlayers.clear();
}