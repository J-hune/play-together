#include "../../include/Game/Menu.h"

std::vector<Button> aggregateButtons(const std::map<GameState, std::vector<Button>>& buttonsMap) {
    std::vector<Button> flattenedButtons;
    for (const auto& pair : buttonsMap) {
        const std::vector<Button>& buttons = pair.second;
        flattenedButtons.insert(flattenedButtons.end(), buttons.begin(), buttons.end());
    }
    return flattenedButtons;
}

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font, Game* game): renderer(renderer), font(font), game(game) {
    // Create menu buttons
    SDL_Color normalColor = {180, 20, 0, 255};
    SDL_Color hoverColor = {255, 0, 255, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    ButtonPosition playButtonPosition = {200, 200, 400, 100};
    auto playButton = Button(renderer, font, playButtonPosition, "Start the Game!", ButtonAction::START, normalColor, hoverColor, textColor, 10);

    buttons[GameState::STOPPED].push_back(playButton);

    auto playButton2 = Button(renderer, font, playButtonPosition, "Resume the Game!", ButtonAction::RESUME, normalColor, hoverColor, textColor, 10);


    // Add a button to stop the game
    ButtonPosition stopButtonPosition = {200, 330, 400, 100};
    auto stopButton = Button(renderer, font, stopButtonPosition, "Stop the Game!", ButtonAction::STOP, normalColor,  hoverColor, textColor, 10);

    buttons[GameState::PAUSED].push_back(playButton2);
    buttons[GameState::PAUSED].push_back(stopButton);
}

void Menu::render() {
    using enum GameState;
    if (game->getGameState() == STOPPED) {
        for (Button& button : buttons[STOPPED]) {
            button.render();
        }
    } else if (game->getGameState() == PAUSED) {
        for (Button& button : buttons[PAUSED]) {
            button.render();
        }
    }

    SDL_Delay(4);
}

void Menu::handleEvent(const SDL_Event& event) {
    using enum GameState;
    if (game->getGameState() == STOPPED) {
        for (Button& button : buttons[STOPPED]) {
            button.handleEvent(event);
            if (button.isButtonClicked()) {
                displayMenu = false;
                button.reset();
            }
        }
    } else if (game->getGameState() == PAUSED) {
        for (Button& button : buttons[PAUSED]) {
            button.handleEvent(event);
            if (button.isButtonClicked()) {
                if (button.getButtonAction() == ButtonAction::RESUME) {
                    displayMenu = false;
                    button.reset();
                } else if (button.getButtonAction() == ButtonAction::STOP) {
                    button.reset();
                    game->stop();
                }
            }
        }
    }
}

bool Menu::isDisplayingMenu() const {
    return displayMenu;
}

void Menu::reset() {
    displayMenu = true;

    // Iterate over buttons by reference
    for (Button& button : aggregateButtons(buttons)) {
        button.reset();
    }
}

void Menu::setDisplayMenu(bool shouldDisplayMenu) {
    displayMenu = shouldDisplayMenu;
}