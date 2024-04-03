#ifndef PLAY_TOGETHER_MEDIATOR_H
#define PLAY_TOGETHER_MEDIATOR_H

#include <string>
#include <SDL.h>
#include <array>
#include <unordered_map>
#include "../Game/Player.h"
#include "../../dependencies/json.hpp"

// Forward declarations
class Game;
class Menu;
class NetworkManager;

enum class GameState {
    RUNNING,
    PAUSED,
    STOPPED
};

/**
 * @class Mediator
 * @brief The Mediator class is used to enable communication between the Menu, Game, and NetworkManager objects.
 *
 * The Mediator pattern is used here to enable communication between the Menu, Game, and NetworkManager objects while avoiding direct dependencies between them.
 * This allows for more flexible and modular design, as each component can communicate through the Mediator without needing to know the details of the others.
 * For example, the Menu can trigger actions in the NetworkManager to start client connections, and the NetworkManager can inform the Menu of server disconnections.
 * All communication between these components goes through the Mediator.
 */
class Mediator {
public:
    /** CONSTRUCTORS **/

    Mediator();


    /** SETTERS **/

    /**
     * @brief Sets the pointer to the associated Game object.
     * @param gamePtr The pointer to the Game object.
     */
    static void setGamePtr(Game *gamePtr);

    /**
     * @brief Sets the pointer to the associated Menu object.
     * @param menuPtr The pointer to the Menu object.
     */
    static void setMenuPtr(Menu *menuPtr);

    /**
     * @brief Sets the pointer to the associated NetworkManager object.
     * @param networkManagerPtr The pointer to the NetworkManager object.
     */
    static void setNetworkManagerPtr(NetworkManager *networkManagerPtr);


    /** PUBLIC METHODS **/

    // NetworkManager methods
    static bool isServerRunning();
    static bool isClientRunning();
    static void startServers();
    static void startClients();
    static void stopServers();
    static void stopClients();
    static void sendPlayerUpdate(uint16_t keyboardStateMask);

    // Menu methods
    static void handleServerDisconnect();

    // Game methods
    static GameState getGameState();
    static void initializeGame(int slot);
    static void stop();
    static void save();
    static void getGameProperties(nlohmann::json &properties);

    // Other methods
    /**
     * @brief Handles a client connection.
     * @param playerID The ID of the player who connected.
     */
    static int handleClientConnect(int playerID);

    /**
     * @brief Handles a client disconnection.
     * @param playerID The ID of the player who disconnected.
     */
    static int handleClientDisconnect(int playerID);

    /**
     * @brief Handles messages received from the network.
     * @param protocol The protocol used to send the message (0 for TCP, 1 for UDP).
     * @param message The message received.
     * @param playerID The ID of the player who sent the message. (0 for server)
     */
    static void handleMessages(int protocol, const std::string &message, int playerID);

    /**
     * @brief Creates a mask of the keyboard state. Each bit represents a key.
     * @return The mask of the keyboard state.
     */
    static uint16_t encodeKeyboardStateMask(const Uint8 *keyboardState);

    /**
     * @brief Decodes a mask of the keyboard state. Each bit represents a key.
     * @param mask The mask of the keyboard state.
     * @param keyStates The array of key states to update.
     */
    static void decodeKeyboardStateMask(uint16_t mask, std::array<int, SDL_NUM_SCANCODES> &keyStates);

private:
    /** ATTRIBUTES **/

    static Game *gamePtr; /**< Pointer to the associated Game object. */
    static Menu *menuPtr; /**< Pointer to the associated Menu object. */
    static NetworkManager *networkManagerPtr; /**< Pointer to the associated NetworkManager object. */
    static const std::array<SDL_Scancode, 7> keyMapping;
    static std::unordered_map<int, std::unordered_map<SDL_Scancode, bool>> playersKeyStates; // Map of player ID to key states

    static void handleKeyboardState(Player *player, std::array<int, SDL_NUM_SCANCODES> &keyStates);

};

#endif //PLAY_TOGETHER_MEDIATOR_H
