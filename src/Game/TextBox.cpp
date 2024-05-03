#include "../../include/Game/TextBox.h"


/** CONSTRUCTORS **/

TextBox::TextBox(SDL_Renderer *renderer, SDL_Rect rect, std::string placeholder, size_t maxLength)
        : renderer(renderer), rect(rect), maxLength(maxLength), placeholder(std::move(placeholder)) {
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
    lastCursorBlink = SDL_GetTicks();
    cursorVisible = true;

    // Handle text input (SDL_TEXTINPUT) and key press (SDL_KEYDOWN)
    if (e.type == SDL_TEXTINPUT) {
        if (text.length() >= maxLength) return;

        // Split the text into two parts: before and after the cursor
        std::string text_before_cursor = text.substr(0, cursorIndex);
        std::string text_after_cursor = text.substr(cursorIndex);

        // Insert the input character between the two parts
        text = text_before_cursor + e.text.text + text_after_cursor;
        moveCursorRight();
    }

    // Handle backspace, left and right arrow keys, and copy-paste shortcuts
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            // Remove the character to the left of the cursor and move the cursor left
            if (cursorIndex > 0) {
                text.erase(cursorIndex - 1, 1);
                moveCursorLeft();
            }

            // If the text is longer than the rectangle
            if (scrollOffset > 0 && static_cast<int>(cursorPosition.x) - scrollOffset + getTextWidth(text.substr(0, 1)) < rect.w - margin) {
                scrollTextRight();
            }
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
            char *clipboard_text = SDL_GetClipboardText();

            // Cut the text if it exceeds the maximum length
            if (text.length() + strlen(clipboard_text) > maxLength) {
                clipboard_text[maxLength - text.length()] = '\0';
            }

            setText(text + clipboard_text);
            SDL_free(clipboard_text);

            // Si le texte est plus long que le rectangle, on scroll le texte de la taille du texte qui dépasse
            while (getTextWidth(text) - scrollOffset > rect.w - margin) {
                scrollTextLeft();
            }

            cursorIndex = static_cast<Uint32>(text.length());
            cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
        }
    }

    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX; int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if the click happened within the textbox boundaries
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            // Calculate cursor position based on click position
            int clickX = mouseX - rect.x + scrollOffset;

            // Find the character index based on click position
            int index = 0;
            for (char c : text) {
                int charWidth = getTextWidth(std::string(1, c));
                if ((clickX -= charWidth) < charWidth / 2) break;
                ++index;
            }

            // Update cursor index and position
            cursorIndex = index;
            cursorPosition.x = getTextWidth(text.substr(0, cursorIndex));
        }
    }
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
    if (cursorIndex < text.length() && static_cast<int>(cursorPosition.x) - scrollOffset + getTextWidth(text.substr(cursorIndex, 1)) > rect.w - margin * 2) {
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
    int first_char_width = getTextWidth(text.substr(0, 1));
    scrollOffset -= first_char_width;
}

void TextBox::scrollTextLeft() {
    // Scroll the text to the left by the width of the last character
    int last_char_width = getTextWidth(text.substr(text.length() - 1));
    scrollOffset += last_char_width;
}

void TextBox::render() {
    SDL_Color placeholder_color = {0, 0, 0, 125};
    std::string display_text = (text.empty()) ? placeholder : text;
    SDL_Color text_color_final = (display_text == placeholder) ? placeholder_color : textColor;

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
    int textX = rect.x + margin - scrollOffset; // Adjust this value to add some padding from the left edge
    int textY = rect.y + rect.h / 2; // Center vertically within the textbox

    // Create a surface and texture for the text
    SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font, display_text.c_str(), text_color_final);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    // Update the text rendering rectangle
    SDL_Rect text_rect = {
            textX,
            textY - text_surface->h / 2, // Center vertically within the textbox
            text_surface->w,
            text_surface->h
    };

    // Clip the text rendering rectangle with SDL_RenderSetClipRect
    SDL_RenderSetClipRect(renderer, &rect);
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
    renderCursor();
    SDL_SetTextInputRect(&text_rect);

    // Reset the clip rectangle
    SDL_RenderSetClipRect(renderer, nullptr);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
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
        SDL_Rect cursor_rect = {
                rect.x + static_cast<int>(cursorPosition.x) - scrollOffset + margin,
                rect.y + rect.h / 4,
                2,
                static_cast<int>(rect.h * 0.5)
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursor_rect);
    }
}

void TextBox::reset() {
    text.clear();
    cursorIndex = 0;
    cursorPosition.x = rect.x;
    scrollOffset = 0;
    cursorVisible = true;
    lastCursorBlink = SDL_GetTicks();
}