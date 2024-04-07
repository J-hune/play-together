//
// Created by anais on 27/03/24.
//

#include <iostream>
#include "../../include/Game/TextBox.h"
void TextBox::updateTextTexture(SDL_Renderer *renderer, SDL_Texture *textTexture, std::string text) {
  {
        // Détruire la texture de texte actuelle
        if (textTexture) {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }

        // Créer une nouvelle texture de texte avec le texte fourni
        SDL_Surface *temp = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (temp) {
            textTexture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            temp = nullptr;
        }
    }
}

void TextBox::handleEvent(const SDL_Event &e) {

    if (e.type == SDL_MOUSEMOTION) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        active = (mouseX >= box.x && mouseX <= box.x + box.w && mouseY >= box.y && mouseY <= box.y + box.h);
    } else if (e.type == SDL_TEXTINPUT && active) {
        // Ajouter le texte saisi à la TextBox
        text += e.text.text;
        updateTextTexture(renderer, textTexture, text);
        std::cout << "Texte saisi : " << text << std::endl; // Afficher dans le terminal
    } else if (e.type == SDL_KEYDOWN && active) {
        // Gérer la suppression de caractères avec la touche retour arrière
        if (e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
            text.pop_back();
            std::cout << "Texte après suppression : " << text << std::endl; // Afficher dans le terminal
            updateTextTexture(renderer, textTexture, text);
        }
    }
}

void TextBox::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &box);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = { box.x + (box.w - textWidth) / 2, box.y + (box.h - textHeight) / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    this->textTexture = textTexture;
    //SDL_FreeSurface(textSurface);
    //SDL_DestroyTexture(textTexture);
}


TextBox::TextBox(SDL_Renderer *renderer, TTF_Font *font, int x, int y, int width, int height, SDL_Color textColor,
                 SDL_Color bgColor) {
    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->renderer = renderer;
}
