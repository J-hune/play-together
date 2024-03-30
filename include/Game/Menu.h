#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <thread>

#include "../Graphics/Button.h"
#include "Game.h"
#include "../Utils/Mediator.h"

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
    /** CONSTRUCTOR **/

    /**
     * @brief Constructor for the Menu class.
     * @param renderer The SDL_Renderer to render the menu.
     * @param fonts A vector of TTF_Font objects for rendering text.
     * @param game The game object.
     * @param quit A pointer to a boolean to control the game loop.
     */
    Menu(SDL_Renderer *renderer, std::vector<TTF_Font *> &fonts, Game *game, bool *quit, Mediator *mediator);


    /** ACCESSORS **/

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

    /** MODIFIERS **/

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

    /** PUBLIC METHODS **/

    /**
     * @brief Reset the menu.
     */
    void reset();

    void onServerDisconnect();
private:
    /** ATTRIBUTES **/

    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    std::vector<TTF_Font *> &fonts; /**< TTF fonts for rendering text. */
    bool displayMenu = true; /**< Flag indicating whether the menu should be displayed. */
    Game *gamePtr; /**< Pointer to the game object. */
    bool *quit; /**< Pointer to a boolean controlling the game loop. */
    MenuAction currentMenuAction = MenuAction::MAIN; /**< Current menu action. */
    std::map<GameStateKey, std::vector<Button>> buttons; /**< Map storing buttons for different game states and menu actions. */
    Mediator *mediatorPtr; /**< Pointer to the network manager mediator. */


    /** ACCESSORS **/

    /**
     * @brief Get the buttons corresponding to the current menu state.
     * @return Reference to the vector of buttons.
     */
    std::vector<Button>& getCurrentMenuButtons();


    /** PRIVATE METHODS **/

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
};

#endif //PLAY_TOGETHER_MENU_H
