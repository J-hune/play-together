#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <thread>
#include <format>

#include "../Graphics/Button.h"
#include "../Utils/Mediator.h"
#include "../Sounds/Music.h"
#include "GameManagers/RenderManager.h"

enum class MenuAction {
    MAIN,
    PLAY,
    CREATE_OR_LOAD_GAME,
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
private:
    /** ATTRIBUTES **/

    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    bool displayMenu = true; /**< Flag indicating whether the menu should be displayed. */
    bool *quitPtr; /**< Pointer to a boolean controlling the game loop. */
    MenuAction currentMenuAction = MenuAction::MAIN; /**< Current menu action. */
    std::map<GameStateKey, std::vector<Button>> buttons; /**< Map storing buttons for different game states and menu actions. */
    Music music; /**< The music played when the menu is displayed. */
    SoundEffect forwardSound = SoundEffect("Menu/forward.wav"); /**< The sound played when the mouse click on a forward button. */
    SoundEffect backSound = SoundEffect("Menu/back.wav"); /**< The sound played when the mouse click on a back button. */
    MessageQueue *messageQueue; /**< Pointer to the message queue for communication between objects. */

public:
    /** CONSTRUCTOR **/

    /**
     * @brief Constructor for the Menu class.
     * @param renderer The SDL_Renderer to render the menu.
     * @param quit A pointer to a boolean to control the game loop.
     * @param music_file_name Menu music file name.
     */
    Menu(SDL_Renderer *renderer, bool *quit, const std::string& music_file_name, MessageQueue *messageQueue);


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
     * @brief Set whether the menu should be displayed or not.
     * @param display_menu True to display the menu, false to hide it.
     */
    void setDisplayMenu(bool display_menu);

    /**
     * @brief Set the current menu action.
     * @param menu_action The menu action to set.
     */
    void setMenuAction(MenuAction menu_action);

    /**
     * @brief Set whether the game loop should quit or not.
     * @param quit_value True to quit the game loop, false otherwise.
     */
    void setQuit(bool quit_value);


    /** METHODS **/

    /**
     * @brief Start playing the menu music.
     */
    void playMusic();

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
     * @brief Reset the menu.
     */
    void reset();

    /**
     * @brief Handle the server disconnect event.
     */
    void onServerDisconnect();

private:
    /** ACCESSORS **/

    /**
     * @brief Get the buttons corresponding to the current menu state.
     * @return Reference to the vector of buttons.
     */
    std::vector<Button>& getCurrentMenuButtons();


    /** PRIVATE METHODS **/

    // Button action handlers
    void handleButtonAction(Button &button);
    void handleStartButton(Button &button, int slot);
    void handleResumeButton(Button &button) const;
    void handleSaveButton(Button &button) const;
    void handleStopButton(Button &button);
    void handleCreateOrLoadGameButton(Button &button);
    void handleJoinHostedGameButton(Button &button) const;
    void handleDeleteSaveButton(Button &button);
    void handleNavigateToMainMenuButton(Button &button);
    void handleNavigateToPlayMenuButton(Button &button);
    void handleQuitButton(Button &button);

    /**
     * @brief Update the save slots displayed on the menu.
     */
    void updateSaveSlots();
};

#endif //PLAY_TOGETHER_MENU_H
