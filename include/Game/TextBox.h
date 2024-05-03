#ifndef PLAY_TOGETHER_TEXTBOX_H
#define PLAY_TOGETHER_TEXTBOX_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_events.h>
#include <string>

#include "../../dependencies/SDL2_gfx/SDL2_gfxPrimitives.h"
#include "../../include/Game/GameManagers/RenderManager.h"

struct cursor {
    unsigned int x;
    unsigned int y;
};

class TextBox {
private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    std::string text;
    std::string placeholder;
    std::vector<TTF_Font *> fonts;
    TTF_Font *font = nullptr;
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color backgroundColor  = {125, 125, 125, 125};
    cursor cursorPosition = {0,0};
    Uint32 cursorIndex = 0;
    int scrollOffset = 0;
    bool cursorVisible = true;
    Uint32 blinkDuration = 600;
    Uint32 lastCursorBlink = SDL_GetTicks();

public:
    /** CONSTRUCTORS **/
    TextBox(SDL_Renderer *renderer, SDL_Rect rect, std::string placeholder);


    /** ACCESSORS **/

    /**
     * @brief Returns the text of the TextBox.
     * @return The text of the TextBox.
     */
    [[nodiscard]] std::string getText() const;


    /** MODIFIERS **/

    /**
     * @brief Set the text of the TextBox.
     * @param value The text to set.
     */
    void setText(const std::string& value);


    /** PUBLIC METHODS **/

    /**
     * @brief Clear the text of the TextBox.
     */
    void emptyText();

    /**
     * @brief Handle events for the TextBox.
     * @param e The SDL_Event to handle.
     */
    void handleEvent(const SDL_Event &e);

    /**
     * @brief Render the TextBox on the screen.
     */
    void render();


    /**
     * @brief Move the cursor to the specified position.
     * @param x The x position.
     * @param y The y position.
     */
    void moveCursor(int x, int y);

private:

    /** PRIVATE METHODS **/

    /**
     * @brief Get the width of the text.
     * @param text The text to get the width.
     * @return The width of the text.
     */
    [[nodiscard]] int getTextWidth(const std::string& text) const;

    /**
     * @brief Move the cursor to the left.
     */
    void moveCursorLeft();

    /**
     * @brief Move the cursor to the right.
     */
    void moveCursorRight();

    /**
     * @brief Scroll the text to the right.
     */
    void scrollTextRight();

    /**
     * @brief Scroll the text to the left.
     */
    void scrollTextLeft();

    /**
     * @brief Render the cursor on the screen.
     */
    void renderCursor();
};



#endif //PLAY_TOGETHER_TEXTBOX_H
