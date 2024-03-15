#include "../../include/Utils/Mediator.h"
#include "../../include/Game/Game.h"
#include "../../include/Game/Menu.h"
#include "../../include/Network/NetworkManager.h"

// Define the static member variables
Game* Mediator::gamePtr = nullptr;
Menu* Mediator::menuPtr = nullptr;
NetworkManager* Mediator::networkManagerPtr = nullptr;
std::unordered_map<int, std::unordered_map<SDL_Scancode, bool>> Mediator::playersKeyStates;
const std::array<SDL_Scancode, 7> Mediator::keyMapping = {
        SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LSHIFT, SDL_SCANCODE_E, SDL_SCANCODE_F
};

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

void Mediator::sendPlayerUpdate(uint16_t keyboardStateMask) {
    // If the server is not running, return
    Mediator::networkManagerPtr->sendPlayerUpdate(keyboardStateMask);
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

GameState Mediator::getGameState() {
    return gamePtr->getGameState();
}

void Mediator::stop() {
    gamePtr->stop();
}

int Mediator::handleClientConnect(int playerID) {
    // Check if the player ID is not already taken by another character.
    if (gamePtr->getPlayer().getPlayerID() == playerID) return -1;
    for (const auto &character : gamePtr->getCharacters()) {
        if (character.getPlayerID() == playerID) {
            return -1; // ID already taken.
        }
    }

    // If the ID is valid and not taken, create a new character for the new player.
    Player newPlayer(playerID, 50, 50, 0.2F, 2, 48, 36);
    gamePtr->addCharacter(newPlayer);

    std::cout << "Mediator: Player " << playerID << " connected" << std::endl;
    return 0;
}

int Mediator::handleClientDisconnect(int playerID) {
    // Find the character with the given player ID and remove it from the game.
    Player *characterPtr = gamePtr->findPlayerById(playerID);
    gamePtr->removeCharacter(characterPtr);

    std::cout << "Mediator: Player " << playerID << " disconnected" << std::endl;
    return 0;
}

void Mediator::handleMessages(const std::string &rawMessage, int playerID) {
    std::cout << "Mediator: Received message: " << rawMessage << " from player " << playerID << std::endl;

    using json = nlohmann::json;
    try {
        // Parse the received message as JSON
        json message = json::parse(rawMessage);

        // Check message type
        MessageType messageType = message["messageType"];


        if (messageType == MessageType::PLAYER_UPDATE) {
            std::cout << "Mediator: Handling player update message" << std::endl;

            // Decode the keyboard state mask
            uint16_t keyboardStateMask = message["keyboardStateMask"];
            int keyStates[SDL_NUM_SCANCODES] = {0};
            decodeKeyboardStateMask(keyboardStateMask, keyStates);

            Player *playerPtr = gamePtr->findPlayerById(playerID);
            std::cout << "Mediator: Player ID: " << playerID << " check:" << playerPtr << std::endl;
            handleKeyboardState(playerPtr, keyStates);
        }

    } catch (const json::parse_error &e) {
        std::cerr << "Mediator: Error parsing message: " << e.what() << std::endl;
    }
}

uint16_t Mediator::encodeKeyboardStateMask(const Uint8 *keyboardState) {
    uint16_t mask = 0;

    // Encoding the keyboard state into a 16-bit mask (9 bits left)
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_Z] || keyboardState[SDL_SCANCODE_SPACE]) << 1; // Up action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_Q]) << 2; // Left action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) << 3; // Right action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) << 4; // Down action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_LSHIFT]) << 5; // Run action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_E]) << 6; // Interact action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_F]) << 7; // Attack action

    return mask;
}

void Mediator::decodeKeyboardStateMask(uint16_t mask, int *keyStates) {
    // Unmasking equivalent keys
    keyStates[SDL_SCANCODE_UP] = (mask & (1 << 1)) != 0;
    keyStates[SDL_SCANCODE_LEFT] = (mask & (1 << 2)) != 0;
    keyStates[SDL_SCANCODE_RIGHT] = (mask & (1 << 3)) != 0;
    keyStates[SDL_SCANCODE_DOWN] = (mask & (1 << 4)) != 0;
    keyStates[SDL_SCANCODE_LSHIFT] = (mask & (1 << 5)) != 0;
    keyStates[SDL_SCANCODE_E] = (mask & (1 << 6)) != 0;
    keyStates[SDL_SCANCODE_F] = (mask & (1 << 7)) != 0;
}

void Mediator::handleKeyboardState(Player *player, const int *keyStates) {
    int playerID = player->getPlayerID();
    SDL_KeyboardEvent keyEvent;

    // Iterate through the keyStates (using the keyMapping array to reduce the number of iterations)
    for (int scancode : keyMapping) {
        keyEvent.keysym.scancode = static_cast<SDL_Scancode>(scancode);
        keyEvent.keysym.sym = SDL_GetKeyFromScancode(keyEvent.keysym.scancode);

        // If the key is pressed and was not pressed before, handle the key down event
        if (keyStates[scancode] == 1 && !playersKeyStates[playerID][keyEvent.keysym.scancode]) {
            gamePtr->handleKeyDownEvent(player, keyEvent);
            playersKeyStates[playerID][keyEvent.keysym.scancode] = true;
        }

        // If the key is released and was pressed before, handle the key up event
        else if (keyStates[scancode] == 0 && playersKeyStates[playerID][keyEvent.keysym.scancode]) {
            gamePtr->handleKeyUpEvent(player, keyEvent);
            playersKeyStates[playerID][keyEvent.keysym.scancode] = false;
        }
    }
}
