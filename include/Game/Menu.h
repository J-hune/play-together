#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include "../Graphics/Button.h"
#include "Game.h"

enum class MenuAction {
    MAIN,
    PLAY,
    LOAD_SAVE,
    START_NEW_GAME,
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
     * @brief Constructor for the Menu class.
     * @param renderer The SDL_Renderer to render the menu.
     * @param font The TTF_Font to render the text on the menu.
     * @param game The game object.
     * @param quit A pointer to a boolean to control the game loop.
     */
    Menu(SDL_Renderer *renderer, TTF_Font *font, Game *game, bool *quit);

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

private:
    /** ATTRIBUTES **/

    SDL_Renderer *renderer;
    TTF_Font *font;
    bool displayMenu = true;
    Game *game;
    bool *quit;
    MenuAction currentMenuAction = MenuAction::MAIN;
    std::map<GameStateKey, std::vector<Button>> buttons;
};

#endif //PLAY_TOGETHER_MENU_H
