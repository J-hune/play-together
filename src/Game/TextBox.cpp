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
/*char TextBox::GetShiftedCharacter(SDL_Keycode keycode) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    bool shiftPressed = (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]);

    if (shiftPressed) {
        switch (keycode) {
            case SDLK_a: return 'A';
            case SDLK_b: return 'B';
            case SDLK_c: return 'C';
            case SDLK_d: return 'D';
            case SDLK_e: return 'E';
            case SDLK_f: return 'F';
            case SDLK_g: return 'G';
            case SDLK_h: return 'H';
            case SDLK_i: return 'I';
            case SDLK_j: return 'J';
            case SDLK_k: return 'K';
            case SDLK_l: return 'L';
            case SDLK_m: return 'M';
            case SDLK_n: return 'N';
            case SDLK_o: return 'O';
            case SDLK_p: return 'P';
            case SDLK_q: return 'Q';
            case SDLK_r: return 'R';
            case SDLK_s: return 'S';
            case SDLK_t: return 'T';
            case SDLK_u: return 'U';
            case SDLK_v: return 'V';
            case SDLK_w: return 'W';
            case SDLK_x: return 'X';
            case SDLK_y: return 'Y';
            case SDLK_z: return 'Z';
            default: return GetSpecialShiftedCharacter(keycode);
        }
    } else {
        return GetSpecialShiftedCharacter(keycode);
    }
}

char TextBox::GetSpecialShiftedCharacter(SDL_Keycode keycode) {
    switch (keycode) {
        case SDLK_1: return '!';
        case SDLK_2: return '@';
        case SDLK_3: return '#';
        case SDLK_4: return '$';
        case SDLK_5: return '%';
        case SDLK_6: return '^';
        case SDLK_7: return '&';
        case SDLK_8: return '*';
        case SDLK_9: return '(';
        case SDLK_0: return ')';
        case SDLK_MINUS: return '_';
        case SDLK_EQUALS: return '+';
        case SDLK_LEFTBRACKET: return '{';
        case SDLK_RIGHTBRACKET: return '}';
        case SDLK_BACKSLASH: return '|';
        case SDLK_SEMICOLON: return '.';
        case SDLK_QUOTE: return '"';
        case SDLK_COMMA: return '<';
        case SDLK_PERIOD: return '>';
        case SDLK_SLASH: return '?';
        case SDLK_BACKQUOTE: return '~';
        default: return static_cast<char>(keycode);
    }
}*/

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
