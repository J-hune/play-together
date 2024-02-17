#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include "../Graphics/Button.h"
#include "Game.h"

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
     */
    Menu(SDL_Renderer* renderer, TTF_Font* font, Game* game);

    /**
     * @brief Render the menu on the screen.
     */
    void render();

    /**
     * @brief Handle events for the menu.
     * @param event The SDL_Event to handle.
     */
    void handleEvent(const SDL_Event& event);

    /**
     * @brief Check if the menu is displaying.
     * @return True if the menu is displaying, false otherwise.
     */
    [[nodiscard]] bool isDisplayingMenu() const;

    /**
     * @brief Set whether the menu should be displayed or not.
     * @param displayMenu True to display the menu, false to hide it.
     */
    void setDisplayMenu(bool displayMenu);

    /**
     * @brief Reset the menu.
     */
    void reset();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool displayMenu = true;
    Game* game;
    std::map<GameState, std::vector<Button>> buttons;
};

#endif //PLAY_TOGETHER_MENU_H
