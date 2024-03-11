#include "../../include/Utils/Mediator.h"
#include "../../include/Game/Game.h"
#include "../../include/Game/Menu.h"
#include "../../include/Network/NetworkManager.h"

/** CONSTRUCTORS **/

Mediator::Mediator() = default;


/** MODIFIERS **/

void Mediator::setGamePtr(Game *game) {
    this->gamePtr = game;
}

void Mediator::setMenuPtr(Menu *menu) {
    this->menuPtr = menu;
}

void Mediator::setNetworkManagerPtr(NetworkManager *networkManager) {
    this->networkManagerPtr = networkManager;
}


/** METHODS **/

void Mediator::startServers() {
    this->networkManagerPtr->startServers();
}

void Mediator::startClients() {
    this->networkManagerPtr->startClients();
}

void Mediator::stopServers() {
    this->networkManagerPtr->stopServers();
}

void Mediator::stopClients() {
    this->networkManagerPtr->stopClients();
}

void Mediator::temporarySendMethod(const std::string &message) const {
    this->networkManagerPtr->temporarySendMethod(message);
}

void Mediator::handleServerDisconnect() {
    this->menuPtr->onServerDisconnect();
}

int Mediator::setCharacterSpriteID(short id) {
    // Check if the character ID is valid (i.e., between 1 and 4)
    if (id < 1 || id > 4) {
        return -1; // Invalid character ID.
    }

    // Check if the ID is already taken by another character.
    if (gamePtr->getPlayer().getSpriteID() == id) return -1;
    for (const auto &character : gamePtr->getCharacters()) {
        if (character.getSpriteID() == id) {
            return -1;
        }
    }

    // If the ID is valid and not taken, set it for the player character.
    this->gamePtr->getPlayer().setSpriteID(id);
    return 0;
}