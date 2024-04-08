//
// Created by anais on 27/03/24.
//

#include <iostream>
#include "../../include/Game/TextBox.h"
#include "../../dependencies/SDL2_gfx/SDL2_gfxPrimitives.h"

void TextBox::updateTextTexture(SDL_Renderer *renderer, SDL_Texture *textTexture, std::string text) {
    // Détruire la texture de texte actuelle
    if (textTexture) {
        std::cout << "il y a deja une texture" << std::endl;
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    // Créer une nouvelle texture de texte avec le texte fourni
    SDL_Surface *temp = TTF_RenderText_Solid(font, getText().c_str(), textColor);
    if (temp) {
        textTexture = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
        temp = nullptr;
    }
}
void TextBox::setText(std::string t)
{
    this->text += t;
}
void TextBox::handleEvent(const SDL_Event &e) {

 if (e.type == SDL_TEXTINPUT ) {
        // Ajouter le texte saisi à la TextBox
        setText(e.text.text);
        updateTextTexture(renderer, textTexture, text);
        std::cout << "Texte saisi : " << text << std::endl; // Afficher dans le terminal
    } else if (e.type == SDL_KEYDOWN) {
        // Gérer la suppression de caractères avec la touche retour arrière
        if (e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
            text.pop_back();
            std::cout << "Texte après suppression : " << text << std::endl; // Afficher dans le terminal
            updateTextTexture(renderer, textTexture, text);
        }
    }
}

void TextBox::render() {

    //SDL_Rect inputBoxRect = {100,200,400,50};
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &box);

    // Dessiner le texte
    if (textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect textRect = {box.x + 5, box.y + (box.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }

    // Dessiner le curseur (optionnel)
    if (cursorVisible && textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_SetRenderDrawColor(renderer, 255, 255,255, 255);
        SDL_RenderDrawLine(renderer, box.x + textWidth + 5, box.y + 5, box.x + textWidth + 5, box.y + box.h - 5);
    }
}



TextBox::TextBox(SDL_Renderer *renderer, int x, int y, int width, int height) {
    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;
    this->renderer = renderer;
}
