#ifndef PLAY_TOGETHER_MEDIATOR_H
#define PLAY_TOGETHER_MEDIATOR_H

#include <string>

// Forward declarations
class Game;
class Menu;
class NetworkManager;

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
    static void startServers();
    static void startClients();
    static void stopServers();
    static void stopClients();
    static void temporarySendMethod(const std::string &message);

    // Menu methods
    static void handleServerDisconnect();

    // Game methods
    static int setCharacterSpriteID(short id);

private:
    /** ATTRIBUTES **/

    static Game *gamePtr; /**< Pointer to the associated Game object. */
    static Menu *menuPtr; /**< Pointer to the associated Menu object. */
    static NetworkManager *networkManagerPtr; /**< Pointer to the associated NetworkManager object. */

};

#endif //PLAY_TOGETHER_MEDIATOR_H
