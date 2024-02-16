#include "../../include/Graphics/Button.h"

Button::Button(SDL_Renderer* renderer, TTF_Font* font, std::string buttonText, int x, int y, int w, int h)
        : renderer(renderer), font(font), buttonText(std::move(buttonText)), x(x), y(y), w(w), h(h) {
}

bool Button::isClicked(int mouseX, int mouseY) const {
    // Check if the mouse coordinates are within the button's boundaries
    return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
}

void Button::render() {
    // Determine the color of the button based on its state (hovered or not)
    SDL_Color color = hovered ? hoverColor : normalColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);

    // Render the text on the button
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttonText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { x + w / 2 - textSurface->w / 2, y + h / 2 - textSurface->h / 2, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    // Free the surface and texture memory to prevent memory leaks
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Button::handleEvent(SDL_Event const& e) {
    if (e.type == SDL_MOUSEMOTION) {
        // Get the current mouse position
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        // Update the hovered state of the button
        hovered = isHovered(mouseX, mouseY);
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        // Get the current mouse position
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        // Check if the button is clicked
        if (isClicked(mouseX, mouseY)) {
            clicked = true;
        }
    }
}

bool Button::isHovered(int mouseX, int mouseY) const {
    // Check if the mouse coordinates are within the button's boundaries
    return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
}

bool Button::isButtonClicked() const {
    // Return the current clicked state of the button
    return clicked;
}

void Button::setHoverColor(SDL_Color color) {
    // Set the color used when the button is hovered
    hoverColor = color;
}

void Button::setNormalColor(SDL_Color color) {
    // Set the normal color of the button
    normalColor = color;
}

void Button::setButtonText(std::string text) {
    // Set the text displayed on the button
    buttonText = std::move(text);
}
