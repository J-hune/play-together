//
// Created by anais on 27/03/24.
//

#ifndef PLAY_TOGETHER_TEXTBOX_H
#define PLAY_TOGETHER_TEXTBOX_H


#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_events.h>
#include <string>

class TextBox {
public:
    TextBox(){}
    TextBox(SDL_Renderer* renderer, int x, int y, int width, int height);

    void handleEvent(const SDL_Event &e);
    void render();

    void updateTextTexture(SDL_Renderer *renderer, SDL_Texture *textTexture, std::string text);

    std::string getText() const { return text; }
    void setText(std::string t);

private:
    SDL_Rect box = {200,500,400,50};
    std::string text;
    TTF_Font* font = TTF_OpenFont("assets/font/arial.ttf", 24);
    SDL_Color textColor = {255,255,255,255};
    SDL_Color bgColor  = {210, 204, 207, 0};
    bool cursorVisible = true;
    bool clicked = false;
    SDL_Event *event;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;

};



#endif //PLAY_TOGETHER_TEXTBOX_H
