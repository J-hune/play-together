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
