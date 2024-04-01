#ifndef PLAY_TOGETHER_BUTTON_H
#define PLAY_TOGETHER_BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <utility>
#include "../../dependencies/SDL2_gfx/SDL2_gfxPrimitives.h"

/**
 * @brief A struct representing the position of a button.
 */
struct ButtonPosition {
    int x;
    int y;
    int w;
    int h;
};

enum class ButtonAction {
    VIEW_GAME,
    RESUME,
    STOP,
    SAVE,
    NONE,
    QUIT,
    CREATE_OR_LOAD_GAME,
    DELETE_SAVE,
    JOIN_HOSTED_GAME,
    NAVIGATE_TO_MENU_MAIN,
    NAVIGATE_TO_MENU_PLAY,
};

/**
 * @class Button
 * @brief A class representing a clickable button in SDL.
 */
class Button {
public:
    /**
     * @brief Constructor for the Button class.
     * @param renderer The SDL_Renderer to render the button.
     * @param font The TTF_Font to render the text on the button.
     * @param position The position of the button.
     * @param value The value associated with the button.
     * @param buttonText The text to be displayed on the button.
     * @param buttonAction The action associated with the button.
     * @param normalColor The color of the button when not hovered.
     * @param hoverColor The color of the button when hovered.
     * @param textColor The color of the text on the button.
     */
    Button(SDL_Renderer *renderer, TTF_Font *font, ButtonPosition position, int value, std::string buttonText,
           ButtonAction buttonAction, SDL_Color normalColor, SDL_Color hoverColor, SDL_Color textColor);

    /**
     * @brief Constructor for the Button class with border radius.
     * @param renderer The SDL_Renderer to render the button.
     * @param font The TTF_Font to render the text on the button.
     * @param value The value associated with the button.
     * @param position The position of the button.
     * @param buttonText The text to be displayed on the button.
     * @param buttonAction The action associated with the button.
     * @param normalColor The color of the button when not hovered.
     * @param hoverColor The color of the button when hovered.
     * @param textColor The color of the text on the button.
     * @param borderRadius The border radius of the button.
     */
    Button(SDL_Renderer *renderer, TTF_Font *font, ButtonPosition position, int value, std::string buttonText,
           ButtonAction buttonAction, SDL_Color normalColor, SDL_Color hoverColor, SDL_Color textColor, short borderRadius);


    /** ACCESSORS **/

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
     * @brief Get the action associated with the button.
     * @return The action of the button.
     */
    [[nodiscard]] ButtonAction getButtonAction() const;

    /**
     * @brief Get the value associated with the button.
     * @return The value of the button.
     */
    [[nodiscard]] int getValue() const;


    /** MODIFIERS **/

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

    /**
     * @brief Set the value associated with the button.
     */
    void setValue(int value);


    /** PUBLIC METHODS **/

    /**
    * @brief Render the button on the screen.
    */
    void render();

    /**
     * @brief Handle mouse events for the button.
     * @param e The SDL_Event to handle.
     */
    void handleEvent(SDL_Event const &e);

    /**
     * @brief Reset the button to its initial state.
     */
    void reset();

private:
    SDL_Renderer *renderer;
    TTF_Font *font;
    std::string buttonText;
    ButtonPosition position;
    int value;
    short borderRadius = 0;
    ButtonAction buttonAction = ButtonAction::NONE;
    SDL_Color normalColor = {0, 255, 0, 255};
    SDL_Color hoverColor = {255, 0, 0, 255};
    SDL_Color textColor = {0, 0, 0, 255};
    bool clicked = false;
    bool hovered = false;
};

#endif //PLAY_TOGETHER_BUTTON_H
