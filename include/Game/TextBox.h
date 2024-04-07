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
    TextBox(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, SDL_Color textColor, SDL_Color bgColor);


    void handleEvent(const SDL_Event &e);
    void render(SDL_Renderer* renderer);

    void updateTextTexture(SDL_Renderer *renderer, SDL_Texture *textTexture, std::string text);

    std::string getText() const { return text; }

private:
    SDL_Rect box;
    std::string text;
    TTF_Font* font = NULL;
    SDL_Color textColor  ;
    SDL_Color bgColor ;
    bool active;
    SDL_Event *event;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;
};



#endif //PLAY_TOGETHER_TEXTBOX_H
