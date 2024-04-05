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

bool Mediator::isServerRunning() {
    return Mediator::networkManagerPtr->isServerRunning();
}

bool Mediator::isClientRunning() {
    return Mediator::networkManagerPtr->isClientRunning();
}

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

void Mediator::sendPlayerUpdate(uint16_t keyboardStateMask) {
    // If the server is not running, return
    Mediator::networkManagerPtr->sendPlayerUpdate(keyboardStateMask);
}

void Mediator::handleServerDisconnect() {
    Mediator::menuPtr->onServerDisconnect();
}

GameState Mediator::getGameState() {
    return gamePtr->getGameState();
}

void Mediator::initializeGame(int slot) {
    gamePtr->initialize(slot);
}

void Mediator::stop() {
    gamePtr->stop();
}

void Mediator::save() {
    gamePtr->saveGame();
}

void Mediator::getGameProperties(nlohmann::json &properties) {
    properties["mapName"] = gamePtr->getLevel().getMapName();
    properties["lastCheckpoint"] = gamePtr->getLevel().getLastCheckpoint();
}

std::vector<Player> Mediator::getCharacters() {
    return gamePtr->getCharacters();
}

int Mediator::handleClientConnect(int playerID) {
    // Check if the player ID is not already taken by another character.
    for (const auto &character : gamePtr->getCharacters()) {
        if (character.getPlayerID() == playerID) {
            return -1; // ID already taken.
        }
    }

    // If the ID is valid and not taken, create a new character for the new player.
    // Get the player's position id based on his position in the players list

    size_t spawnIndex = gamePtr->getCharacters().size();
    Level level = gamePtr->getLevel();
    Point spawnPoint = level.getSpawnPoints(level.getLastCheckpoint())[spawnIndex];
    Player newPlayer(playerID, spawnPoint,48, 36);
    gamePtr->addCharacter(newPlayer);

    std::cout << "Mediator: Player " << playerID << " connected" << std::endl;
    return 0;
}

int Mediator::handleClientDisconnect(int playerID) {
    // Find the character with the given player ID and remove it from the game.
    Player const *characterPtr = gamePtr->findPlayerById(playerID);
    gamePtr->removeCharacter(characterPtr);

    std::cout << "Mediator: Player " << playerID << " disconnected" << std::endl;
    return 0;
}

void Mediator::handleMessages(int protocol, const std::string &rawMessage, int playerID) {
    std::cout << "Mediator: Received message: " << rawMessage << " from player " << playerID << std::endl;

    using json = nlohmann::json;
    try {

    // Parse the received message as JSON
    json message = json::parse(rawMessage);

    // If the application is a server, broadcast the message to all clients (except the sender) with the protocol used by the sender
    if (networkManagerPtr->isServerRunning()) {
        message["playerID"] = playerID;
        Mediator::networkManagerPtr->broadcastMessage(protocol, message.dump(), playerID);
    }

    // Check message type and handle it accordingly
    std::string messageType = message["messageType"];
    std::cout << "Mediator: Handling " << messageType << " message" << std::endl;

    if (messageType == "playerUpdate") {
        // Get the player ID from the message if it exists, otherwise use the playerID parameter
        int playerSocketID = (message.contains("playerID") ? (int)message["playerID"] : playerID);

        // Decode the keyboard state mask
        uint16_t keyboardStateMask = message["keyboardStateMask"];
        std::array<int, SDL_NUM_SCANCODES> keyStates = {0};
        decodeKeyboardStateMask(keyboardStateMask, keyStates);

        Player *playerPtr = gamePtr->findPlayerById(playerSocketID);
        handleKeyboardState(playerPtr, keyStates);
    }

    else if (messageType == "playerConnect") {
        int playerSocketID = message["playerID"];

        // Get the player's spawn point based on his position in the players list
        size_t spawnIndex = gamePtr->getCharacters().size();
        Level level = gamePtr->getLevel();
        Point spawnPoint = level.getSpawnPoints(level.getLastCheckpoint())[spawnIndex];

        gamePtr->addCharacter(Player(playerSocketID, spawnPoint, 48, 36));
        std::cout << "Mediator: Player " << playerSocketID << " added to the game" << std::endl;
    }

    else if (messageType == "playerDisconnect") {
        int playerSocketID = message["playerID"];
        gamePtr->removeCharacter(gamePtr->findPlayerById(playerSocketID));
        std::cout << "Mediator: Player " << playerSocketID << " removed from the game" << std::endl;
    }

    else if (messageType == "gameProperties") {
        std::cout << "Mediator: Game properties received: " << message << std::endl;
        gamePtr->setLevel(message["mapName"]);
        gamePtr->getLevel().setLastCheckpoint(message["lastCheckpoint"]);
    }

    else if (messageType == "playerList") {
        std::cout << "Mediator: Players list received: " << message["players"] << std::endl;

        json playersArray = message["players"];
        size_t spawnIndex = gamePtr->getCharacters().size();
        for (const auto &player : playersArray) {
            int receivedPlayerID = player["playerID"];

            Level level = gamePtr->getLevel();
            Point spawnPoint = level.getSpawnPoints(level.getLastCheckpoint())[spawnIndex];
            Player newPlayer(receivedPlayerID, spawnPoint, 48, 36);
            if (player.contains("x")) newPlayer.setX(player["x"]);
            if (player.contains("y")) newPlayer.setY(player["y"]);
            if (player.contains("moveX")) newPlayer.setMoveX(player["moveX"]);
            if (player.contains("moveY")) newPlayer.setMoveY(player["moveY"]);

            if (receivedPlayerID == -1) newPlayer.setSpriteTextureByID(3);
            else if (receivedPlayerID == 0) newPlayer.setSpriteTextureByID(2);

            gamePtr->addCharacter(newPlayer);
            spawnIndex++;
        }
    }

    } catch (const json::parse_error &e) {
        std::cerr << "Mediator: Error parsing message: " << e.what() << std::endl;
    }
}

uint16_t Mediator::encodeKeyboardStateMask(const Uint8 *keyboardState) {
    uint16_t mask = 0;

    // Encoding the keyboard state into a 16-bit mask (9 bits left)
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_SPACE]) << 1; // Up action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) << 2; // Left action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) << 3; // Right action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) << 4; // Down action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_LSHIFT]) << 5; // Run action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_E]) << 6; // Interact action
    mask |= static_cast<uint16_t>(keyboardState[SDL_SCANCODE_F]) << 7; // Attack action

    return mask;
}

void Mediator::decodeKeyboardStateMask(uint16_t mask, std::array<int, SDL_NUM_SCANCODES> &keyStates) {
    // Unmasking equivalent keys
    keyStates[SDL_SCANCODE_UP] = (mask & (1 << 1)) != 0;
    keyStates[SDL_SCANCODE_LEFT] = (mask & (1 << 2)) != 0;
    keyStates[SDL_SCANCODE_RIGHT] = (mask & (1 << 3)) != 0;
    keyStates[SDL_SCANCODE_DOWN] = (mask & (1 << 4)) != 0;
    keyStates[SDL_SCANCODE_LSHIFT] = (mask & (1 << 5)) != 0;
    keyStates[SDL_SCANCODE_E] = (mask & (1 << 6)) != 0;
    keyStates[SDL_SCANCODE_F] = (mask & (1 << 7)) != 0;
}

void Mediator::handleKeyboardState(Player *player, std::array<int, SDL_NUM_SCANCODES> &keyStates) {
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
            Game::handleKeyUpEvent(player, keyEvent);
            playersKeyStates[playerID][keyEvent.keysym.scancode] = false;
        }
    }
}
