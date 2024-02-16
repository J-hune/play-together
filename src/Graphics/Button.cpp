#include "../../include/Graphics/Button.h"

// Function Color rgba to Uint32
Uint32 RGBA(Uint32 r, Uint32 g, Uint32 b, Uint32 a) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

Button::Button(SDL_Renderer* renderer, TTF_Font* font, ButtonPosition position, std::string buttonText, SDL_Color normalColor, SDL_Color hoverColor, SDL_Color textColor)
        : renderer(renderer), font(font), buttonText(std::move(buttonText)), position(position), normalColor(normalColor), hoverColor(hoverColor), textColor(textColor)  {
}

Button::Button(SDL_Renderer *renderer, TTF_Font *font, ButtonPosition position, std::string buttonText, SDL_Color normalColor, SDL_Color hoverColor, SDL_Color textColor, short borderRadius)
        : renderer(renderer), font(font), buttonText(std::move(buttonText)), position(position),
          borderRadius(borderRadius), normalColor(normalColor), hoverColor(hoverColor), textColor(textColor) {
}

void Button::render() {
    // Determine the color of the button based on its state (hovered or not)
    SDL_Color color = hovered ? hoverColor : normalColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    roundedBoxColor(renderer, (short)position.x, (short)position.y, (short)(position.x + position.w), (short)(position.y + position.h), borderRadius, RGBA(color.r, color.g, color.b, color.a));

    // Render the text on the button
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, buttonText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { position.x + position.w / 2 - textSurface->w / 2, position.y + position.h / 2 - textSurface->h / 2, textSurface->w, textSurface->h };

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
        if (isHovered(mouseX, mouseY)) {
            clicked = true;
        }
    }
}

bool Button::isHovered(int mouseX, int mouseY) const {
    // Check if the mouse coordinates are within the button's boundaries, considering the border radius
    if (mouseX >= position.x && mouseX <= position.x + position.w && mouseY >= position.y && mouseY <= position.y + position.h) {
        // Check if the mouse is within the rectangular area of the button minus the border radius
        if (mouseX >= position.x + borderRadius && mouseX <= position.x + position.w - borderRadius &&
            mouseY >= position.y + borderRadius && mouseY <= position.y + position.h - borderRadius) {
            return true;
        }
        // Check if the mouse is within the circle area of each corner (border radius)
        for (int i = 0; i < 4; ++i) {
            int cx = (i % 2 == 0) ? position.x + borderRadius : position.x + position.w - borderRadius;
            int cy = (i < 2) ? position.y + borderRadius : position.y + position.h - borderRadius;
            // Calculate the distance from the mouse to the center of the circle
            int dx = mouseX - cx;
            int dy = mouseY - cy;
            // Check if the distance is within the circle's radius
            if (dx * dx + dy * dy <= borderRadius * borderRadius) {
                return true;
            }
        }
    }
    return false;
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

void Button::setBorderRadius(short radius) {
    // Set the border radius of the button
    borderRadius = radius;
}

void Button::reset() {
    clicked = false;
    hovered = false;
}