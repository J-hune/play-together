#ifndef PLAY_TOGETHER_BUTTON_H
#define PLAY_TOGETHER_BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <utility>
#include <SDL2_gfxPrimitives.h>

/**
 * @brief A class representing a clickable button in SDL.
 */
class Button {
public:
    /**
     * @brief Constructor for Button class.
     * @param renderer The SDL_Renderer to render the button.
     * @param font The TTF_Font to render the button text.
     * @param buttonText The text displayed on the button.
     * @param x The x-coordinate of the button's top-left corner.
     * @param y The y-coordinate of the button's top-left corner.
     * @param w The width of the button.
     * @param h The height of the button.
     */
    [[maybe_unused]] Button(SDL_Renderer* renderer, TTF_Font* font, std::string buttonText, int x, int y, int w, int h);

    /**
     * @brief Constructor for Button class.
     * @param renderer The SDL_Renderer to render the button.
     * @param font The TTF_Font to render the button text.
     * @param buttonText The text displayed on the button.
     * @param x The x-coordinate of the button's top-left corner.
     * @param y The y-coordinate of the button's top-left corner.
     * @param w The width of the button.
     * @param h The height of the button.
     * @param borderRadius The border radius of the button.
     */
    [[maybe_unused]] Button(SDL_Renderer* renderer, TTF_Font* font, std::string buttonText, int x, int y, int w, int h, short borderRadius);

    /**
     * @brief Render the button on the screen.
     */
    void render();

    /**
     * @brief Handle mouse events for the button.
     * @param e The SDL_Event to handle.
     */
    void handleEvent(SDL_Event const& e);

    /**
     * @brief Check if the button is hovered.
     * @param mouseX The x-coordinate of the mouse pointer.
     * @param mouseY The y-coordinate of the mouse pointer.
     * @return True if the button is hovered, false otherwise.
     */
    [[nodiscard]] bool isHovered(int mouseX, int mouseY) const;

    /**
     * @brief Check if the button is clicked.
     * @return True if the button is clicked, false otherwise.
     */
    [[nodiscard]] bool isButtonClicked() const;

    /**
     * @brief Set the color of the button when hovered.
     * @param color The SDL_Color representing the hover color.
     */
    void setHoverColor(SDL_Color color);

    /**
     * @brief Set the normal color of the button.
     * @param color The SDL_Color representing the normal color.
     */
    void setNormalColor(SDL_Color color);

    /**
     * @brief Set the text displayed on the button.
     * @param text The new text to be displayed on the button.
     */
    void setButtonText(std::string text);

    /**
     * @brief Set the border radius of the button.
     * @param radius The new border radius of the button.
     */
    void setBorderRadius(short radius);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::string buttonText;
    int x;
    int y;
    int w;
    int h;
    short borderRadius = 0;
    bool clicked = false;
    bool hovered = false;
    SDL_Color normalColor = { 0, 255, 0, 255 };
    SDL_Color hoverColor = { 255, 0, 0, 255 };
};

#endif //PLAY_TOGETHER_BUTTON_H
