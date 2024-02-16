#ifndef PLAY_TOGETHER_MENU_H
#define PLAY_TOGETHER_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "../Graphics/Button.h"

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
     * @brief Check if the game should start.
     * @return True if the game should start, false otherwise.
     */
    bool shouldStartGame() const;

    /**
     * @brief Reset the menu.
     */
    void reset();

    /**
     * @brief Set whether the game should start.
     * @param shouldStartGame True if the game should start, false otherwise.
     */
    void setShouldStartGame(bool shouldStartGame);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool gameShouldStart = false;
    Button playButton;
};

#endif //PLAY_TOGETHER_MENU_H
