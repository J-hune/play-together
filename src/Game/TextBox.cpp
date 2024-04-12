//
// Created by anais on 27/03/24.
//

#include <iostream>
#include "../../include/Game/TextBox.h"
#include "../../dependencies/SDL2_gfx/SDL2_gfxPrimitives.h"


void TextBox::setText(std::string t)
{
    this->text += t;
}
void TextBox::emptyText()
{
    this->text.clear();
}

void TextBox::handleEvent(const SDL_Event &e) {

    if (e.type == SDL_TEXTINPUT ) {
        setText(e.text.text);
        std::cout << "Texte saisi : " << text << std::endl;
    } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
            text.pop_back();
            std::cout << "Texte après suppression : " << text << std::endl; // Afficher dans le terminal
        }else if (e.key.keysym.sym == SDLK_LEFT) {
            moveCursorLeft();
        } else if (e.key.keysym.sym == SDLK_RIGHT) {
            moveCursorRight();
        }
    }
}

void TextBox::render() {
    SDL_Color backgroundColor = {125, 125, 125, 125};
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color textColorInit = {0, 0, 0, 125};

    std::string defaultText = "IP adress";

    std::string displayText = (!text.empty()) ? getText() : defaultText;
    SDL_Color textColorFinal = (displayText == defaultText) ? textColorInit : textColor;
    roundedBoxRGBA(
            renderer,
            static_cast<short>(box.x),
            static_cast<short>(box.y),
            static_cast<short>(box.x + box.w),
            static_cast<short>(box.y + box.h),
            0,
            backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a
    );

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, displayText.c_str(), textColorFinal);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {
            box.x + box.w / 2 - textSurface->w / 2,
            box.y + box.h / 2 - textSurface->h / 2,
            textSurface->w, textSurface->h
    };

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    renderCursor();
    SDL_SetTextInputRect(&textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void TextBox::moveCursor(int x, int y) {
    cursorPosition.x = x - box.x;
    cursorPosition.y = y - box.y;
}

void TextBox::moveCursorLeft() {
    if (cursorPosition.x > 0) {
        cursorPosition.x --;
    }
}

void TextBox::moveCursorRight() {
    if (cursorPosition.x < text.length()) {
        cursorPosition.x ++;
    }
}

int TextBox::getTextWidth() {
    int textWidth = 0;
    if (!text.empty()) {
        TTF_SizeUTF8(font, text.c_str(), &textWidth, nullptr);
    }
    return textWidth;
}

void TextBox::renderCursor() {

    if (text.empty() || cursorPosition.x < 0 || cursorPosition.y < 0 || cursorPosition.x > box.w ||
        cursorPosition.y > box.h) {
        return;
    }

    bool showCursor = true;
    int cursorToggleInterval = 500;
    int lastToggleTime = SDL_GetTicks();

    if (SDL_GetTicks() - lastToggleTime >= cursorToggleInterval)
    {
        showCursor = !showCursor;
        lastToggleTime = SDL_GetTicks();
    }

    if (showCursor)
    {
        SDL_Rect CursorQuad = {200, 200, 10, 20};
        SDL_Color cursorColor = {0,0,0,255};
        SDL_RenderFillRect(renderer, &CursorQuad);
        SDL_Surface* text_surface = TTF_RenderText_Blended(font, "|", cursorColor);
        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(renderer, text_surface);
        int textWidth = getTextWidth();
        int	TexWidth = box.x + box.w / 2 + textWidth / 2;
        int	TexHeight = box.y + box.h - 2;;
        SDL_QueryTexture(pTexture,NULL,NULL, &TexWidth, &TexHeight);
        SDL_Rect positionQuad = {box.x + box.w / 2 + textWidth / 2,box.y+10, TexWidth, TexHeight};
        SDL_RenderCopy(renderer, pTexture, NULL, &positionQuad);

        SDL_DestroyTexture(pTexture);
        SDL_FreeSurface(text_surface);


    }

}


TextBox::TextBox(SDL_Renderer *renderer, int x, int y, int width, int height) {
    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;
    this->renderer = renderer;
}
