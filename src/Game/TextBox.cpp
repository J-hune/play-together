#include <utility>

#include "../../include/Game/TextBox.h"


/** CONSTRUCTORS **/

TextBox::TextBox(SDL_Renderer *renderer, SDL_Rect rect, std::string placeholder) : renderer(renderer), rect(rect), placeholder(std::move(placeholder)) {
    fonts = RenderManager::getFonts();
    font = fonts[0];

    // Set the initial cursor position to the left edge of the textbox
    cursorPosition.x = rect.x;
}


/** ACCESSORS **/

std::string TextBox::getText() const {
    return text;
}


/** MODIFIERS **/

void TextBox::setText(const std::string& value) {
    text = value;
}

void TextBox::emptyText() {
    text.clear();
}

int TextBox::getTextWidth(const std::string& val) const {
    int width = 0;
    TTF_SizeUTF8(font, val.c_str(), &width, nullptr);
    return width;
}

void TextBox::handleEvent(const SDL_Event &e) {
    // Handle text input (SDL_TEXTINPUT) and key press (SDL_KEYDOWN)
    if (e.type == SDL_TEXTINPUT) {
        // Split the text into two parts: before and after the cursor
        std::string textBeforeCursor = text.substr(0, cursorIndex);
        std::string textAfterCursor = text.substr(cursorIndex);

        // Insert the input character between the two parts
        text = textBeforeCursor + e.text.text + textAfterCursor;
        moveCursorRight();
    }

    // Handle backspace, left and right arrow keys, and copy-paste shortcuts
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            text.pop_back();
            moveCursorLeft();

            // If the text is longer than the rectangle, scroll the text
            if (scrollOffset > 0) scrollTextRight();
        }

        // Delete key to remove character to the right of the cursor
        else if (e.key.keysym.sym == SDLK_DELETE && !text.empty() && cursorIndex < text.length()) {
            text.erase(cursorIndex, 1);
        }

        // Move the cursor left or right
        else if (e.key.keysym.sym == SDLK_LEFT) {
            moveCursorLeft();
        } else if (e.key.keysym.sym == SDLK_RIGHT) {
            moveCursorRight();
        }

        // Shortcut for copying text to the clipboard
        else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
            SDL_SetClipboardText(text.c_str());
        }

        // Shortcut for pasting text from the clipboard
        else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
            char *clipboardText = SDL_GetClipboardText();
            setText(text + clipboardText);
            SDL_free(clipboardText);

            // Si le texte est plus long que le rectangle, on scroll le texte de la taille du texte qui dépasse
            while (getTextWidth(text) - scrollOffset > rect.w) {
                scrollTextLeft();
            }

            cursorIndex = static_cast<Uint32>(text.length());
            cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
        }
    }
}

void TextBox::moveCursor(int x, int y) {
    cursorPosition.x = x - rect.x;
    cursorPosition.y = y - rect.y;
}

void TextBox::moveCursorLeft() {
    if (cursorIndex > 0 && static_cast<int>(cursorPosition.x) > scrollOffset) {
        cursorIndex--;
        cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
    }

    else if (cursorIndex > 0) {
        cursorIndex--;
        scrollTextRight();
        cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
    }
}

void TextBox::moveCursorRight() {
    // If there's more text to the right and moving the cursor right would exceed the textbox width
    if (cursorIndex < text.length() && static_cast<int>(cursorPosition.x) - scrollOffset + getTextWidth(text.substr(cursorIndex, 1)) > rect.w) {
        cursorIndex++;
        scrollTextLeft();
        cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
    }

    // If there's more text to the right and moving the cursor right would not exceed the textbox width
    else if (cursorIndex < text.length()) {
        cursorIndex++;
        cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
    }
}

void TextBox::scrollTextRight() {
    // Scroll the text to the right by the width of the first character
    int firstCharWidth = getTextWidth(text.substr(0, 1));
    scrollOffset -= firstCharWidth;
}

void TextBox::scrollTextLeft() {
    // Scroll the text to the left by the width of the last character
    int lastCharWidth = getTextWidth(text.substr(text.length() - 1));
    scrollOffset += lastCharWidth;
}

void TextBox::render() {
    SDL_Color textColorInit = {0, 0, 0, 125};
    std::string displayText = (text.empty()) ? placeholder : text;
    SDL_Color textColorFinal = (displayText == placeholder) ? textColorInit : textColor;

    // Render the background of the textbox
    roundedBoxRGBA(
            renderer,
            static_cast<Sint16>(rect.x),
            static_cast<Sint16>(rect.y),
            static_cast<Sint16>(rect.x + rect.w),
            static_cast<Sint16>(rect.y + rect.h),
            0,
            backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a
    );

    // Calculate the position to render the text (align left, but not glued to the left edge)
    int textX = rect.x - scrollOffset; // Adjust this value to add some padding from the left edge
    int textY = rect.y + rect.h / 2; // Center vertically within the textbox

    // Create a surface and texture for the text
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, displayText.c_str(), textColorFinal);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Update the text rendering rectangle
    SDL_Rect textRect = {
            textX,
            textY - textSurface->h / 2, // Center vertically within the textbox
            textSurface->w,
            textSurface->h
    };

    // Clip the text rendering rectangle with SDL_RenderSetClipRect
    SDL_RenderSetClipRect(renderer, &rect);
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    renderCursor();
    SDL_SetTextInputRect(&textRect);

    SDL_RenderSetClipRect(renderer, nullptr);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


void TextBox::renderCursor() {
    if (text.empty() || cursorIndex > text.length() || static_cast<int>(cursorPosition.y) > rect.h) {
        return;
    }

    if (SDL_GetTicks() - lastCursorBlink >= blinkDuration) {
        cursorVisible = !cursorVisible;
        lastCursorBlink = SDL_GetTicks();
    }

    if (cursorVisible) {
        // Calculate the cursor position based on the text width
        SDL_Rect cursorRect = {
                rect.x + static_cast<int>(cursorPosition.x) - scrollOffset,
                rect.y + rect.h / 4,
                2,
                static_cast<int>(rect.h * 0.5)
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
}