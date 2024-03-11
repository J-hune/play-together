#include "../../include/Utils/Mediator.h"
#include "../../include/Game/Game.h"
#include "../../include/Game/Menu.h"
#include "../../include/Network/NetworkManager.h"

// Define the static member variables
Game* Mediator::gamePtr = nullptr;
Menu* Mediator::menuPtr = nullptr;
NetworkManager* Mediator::networkManagerPtr = nullptr;

/** CONSTRUCTORS **/

Mediator::Mediator() = default;


/** MODIFIERS **/

void Mediator::setGamePtr(Game *game) {
    Mediator::gamePtr = game;
}

void Mediator::setMenuPtr(Menu *menu) {
    Mediator::menuPtr = menu;
}

void Mediator::setNetworkManagerPtr(NetworkManager *networkManager) {
    Mediator::networkManagerPtr = networkManager;
}


/** METHODS **/

void Mediator::startServers() {
    Mediator::networkManagerPtr->startServers();
}

void Mediator::startClients() {
    Mediator::networkManagerPtr->startClients();
}

void Mediator::stopServers() {
    Mediator::networkManagerPtr->stopServers();
}

void Mediator::stopClients() {
    Mediator::networkManagerPtr->stopClients();
}

void Mediator::temporarySendMethod(const std::string &message) {
    Mediator::networkManagerPtr->temporarySendMethod(message);
}

void Mediator::handleServerDisconnect() {
    Mediator::menuPtr->onServerDisconnect();
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
    gamePtr->getPlayer().setSpriteID(id);
    return 0;
}