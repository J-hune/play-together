#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <thread>

#include "../Graphics/Button.h"
#include "Game.h"

#ifdef _WIN32
#include <winsock2.h>
#include "../Network/WIN32/TCPServer.h"
#include "../Network/WIN32/TCPClient.h"
#include "../Network/WIN32/UDPServer.h"
#include "../Network/WIN32/UDPClient.h"
#else
#include "../Network/Unix/TCPServer.h"
#include "../Network/Unix/TCPClient.h"
#include "../Network/Unix/UDPServer.h"
#include "../Network/Unix/UDPClient.h"
#endif

constexpr unsigned int MAX_JSON_SIZE = 1024;

enum class MenuAction {
    MAIN,
    PLAY,
    LOAD_SAVE,
    START_NEW_GAME,
    HOST_GAME,
    JOIN_GAME,
};

struct GameStateKey {
    GameState state;
    MenuAction action;

    // Define the spaceship operator for GameStateKey
    auto operator<=>(const GameStateKey &other) const = default;
};

/**
 * @class Menu
 * @brief Represents the game menus including the main menu and pause menu
 */
class Menu {
public:
    /**
     * @brief Constructor for the Menu class.
     * @param renderer The SDL_Renderer to render the menu.
     * @param font The TTF_Font to render the text on the menu.
     * @param game The game object.
     * @param quit A pointer to a boolean to control the game loop.
     */
    Menu(SDL_Renderer *renderer, TTF_Font *font, Game *game, bool *quit);

    ~Menu();

    /**
     * @brief Render the menu on the screen.
     */
    void render();

    /**
     * @brief Handle events for the menu.
     * @param event The SDL_Event to handle.
     */
    void handleEvent(const SDL_Event &event);

    /**
     * @brief Check if the menu is displaying.
     * @return True if the menu is displaying, false otherwise.
     */
    [[nodiscard]] bool isDisplayingMenu() const;

    /**
     * @brief Get the current menu action.
     * @return The current menu action.
     */
    [[nodiscard]] MenuAction getCurrentMenuAction() const;

    /**
     * @brief Set whether the menu should be displayed or not.
     * @param displayMenu True to display the menu, false to hide it.
     */
    void setDisplayMenu(bool displayMenu);

    /**
     * @brief Set the current menu action.
     * @param menuAction The menu action to set.
     */
    void setMenuAction(MenuAction menuAction);

    /**
     * @brief Set whether the game loop should quit or not.
     * @param quit_value True to quit the game loop, false otherwise.
     */
    void setQuit(bool quit_value);

    /**
     * @brief Reset the menu.
     */
    void reset();

private:
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    TTF_Font *font; /**< TTF font for rendering text. */
    bool displayMenu = true; /**< Flag indicating whether the menu should be displayed. */
    Game *game; /**< Pointer to the game object. */
    bool *quit; /**< Pointer to a boolean controlling the game loop. */
    MenuAction currentMenuAction = MenuAction::MAIN; /**< Current menu action. */
    TCPServer tcpServer; /**< TCP server instance for network communication. */
    TCPClient tcpClient; /**< TCP client instance for network communication. */
    UDPServer udpServer; /**< UDP server instance for network communication. */
    UDPClient udpClient; /**< UDP client instance for network communication. */
    std::map<GameStateKey, std::vector<Button>> buttons; /**< Map storing buttons for different game states and menu actions. */
    std::unique_ptr<std::jthread> serverTCPThreadPtr; /**< Pointer to the TCP server thread. */
    std::unique_ptr<std::jthread> clientTCPThreadPtr; /**< Pointer to the TCP client thread. */
    std::unique_ptr<std::jthread> serverUDPThreadPtr; /**< Pointer to the UDP server thread. */
    std::unique_ptr<std::jthread> clientUDPThreadPtr; /**< Pointer to the UDP client thread. */
    std::mutex clientAddressesMutex = {}; /**< Mutex to protect the client addresses map. */

#ifdef _WIN32
    std::map<SOCKET, sockaddr_in> clientAddresses; /**< Map storing client addresses. */
#else
    std::map<int, sockaddr_in> clientAddresses; /**< Map storing client addresses. */
#endif

    /**
     * @brief Wait for a connection to be established.
     */
    void startServer();

    /**
     * @brief Start the client to connect to the server.
     */
    void startClient();

    /**
     * @brief Callback function to handle server disconnect event.
     */
    void onServerDisconnect();

    /**
     * @brief Get the buttons corresponding to the current menu state.
     * @return Reference to the vector of buttons.
     */
    std::vector<Button>& getCurrentMenuButtons();

    // Button action handlers
    void handleButtonAction(Button &button);
    void handleStartButton(Button &button);
    void handleResumeButton(Button &button);
    void handleStopButton(Button &button);
    void handleHostGameButton(Button &button);
    void handleJoinGameButton(Button &button);
    void handleNavigateToMainMenuButton(Button &button);
    void handleNavigateToPlayMenuButton(Button &button);
    void handleSendMessageButton(Button &button);
    void handleNavigateToLoadSaveMenuButton(Button &button);
    void handleNavigateToStartNewGameMenuButton(Button &button);
    void handleQuitButton([[maybe_unused]] Button &button);

    template<typename SocketType>
    void cleanupThreads(std::unique_ptr<std::jthread> &threadPtr, SocketType &socket) const;
};

#endif //PLAY_TOGETHER_MENU_H
