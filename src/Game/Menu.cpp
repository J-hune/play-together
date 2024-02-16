#include "../../include/Game/Menu.h"

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font)
        : renderer(renderer), font(font), playButton(renderer, font, {200, 200, 400, 100}, "Play My Game!", {180, 20, 0, 255}, {255, 0, 255, 255}, {255, 255, 255, 255}) {
    // Create menu buttons
    SDL_Color normalColor = {180, 20, 0, 255};
    SDL_Color hoverColor = {255, 0, 255, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    ButtonPosition playButtonPosition = {200, 200, 400, 100};
    playButton = Button(renderer, font, playButtonPosition, "Play My Game!", normalColor, hoverColor, textColor, 10);
    playButton.setButtonText("Play My Game!");
}

void Menu::render() {
    playButton.render();
}

void Menu::handleEvent(const SDL_Event& event) {
    playButton.handleEvent(event);
    if (playButton.isButtonClicked()) {
        gameShouldStart = true;
    }
}

bool Menu::shouldStartGame() const {
    return gameShouldStart;
}

void Menu::reset() {
    gameShouldStart = false;
    playButton.reset();
}

void Menu::setShouldStartGame(bool shouldStartGame) {
    gameShouldStart = shouldStartGame;
}