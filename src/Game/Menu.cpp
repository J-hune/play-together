#include "../../include/Game/Menu.h"

/**
 * @brief Implementation of the Menu class responsible for rendering and handling events for the game menus.
 */

// Helper function to flatten a map of buttons into a vector of buttons
std::vector<Button> aggregateButtons(const std::map<GameState, std::vector<Button>>& buttonsMap) {
    std::vector<Button> flattenedButtons;
    for (const auto& [state, buttons] : buttonsMap) {
        flattenedButtons.insert(flattenedButtons.end(), buttons.begin(), buttons.end());
    }
    return flattenedButtons;
}

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font, Game* game): renderer(renderer), font(font), game(game) {
    // Create menu buttons
    SDL_Color normal_color = {180, 20, 0, 255};
    SDL_Color hover_color = {255, 0, 255, 255};
    SDL_Color text_color = {255, 255, 255, 255};
    ButtonPosition play_button_position = {200, 200, 400, 100};
    auto play_button = Button(renderer, font, play_button_position, "Start the Game!", ButtonAction::START, normal_color, hover_color, text_color, 10);

    buttons[GameState::STOPPED].push_back(play_button);

    auto play_button2 = Button(renderer, font, play_button_position, "Resume the Game!", ButtonAction::RESUME, normal_color, hover_color, text_color, 10);


    // Add a button to stop the game
    ButtonPosition stop_button_position = {200, 330, 400, 100};
    auto stop_button = Button(renderer, font, stop_button_position, "Stop the Game!", ButtonAction::STOP, normal_color,  hover_color, text_color, 10);

    buttons[GameState::PAUSED].push_back(play_button2);
    buttons[GameState::PAUSED].push_back(stop_button);
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
    // Get the buttons corresponding to the current game state
    auto& current_buttons = buttons[game->getGameState()];

    // Iterate through each button
    for (Button& button : current_buttons) {
        // Handle events for the button
        button.handleEvent(event);

        // Check if the button is clicked
        if (button.isButtonClicked()) {
            switch (button.getButtonAction()) {
                using enum ButtonAction;
                case START:
                case RESUME:
                    displayMenu = false;
                    button.reset();
                    break;
                case STOP:
                    button.reset();
                    game->stop();
                    break;
                default:
                    break;
            }
        }
    }
}

bool Menu::isDisplayingMenu() const {
    return displayMenu;
}

void Menu::setDisplayMenu(bool display_menu) {
    displayMenu = display_menu;
}

void Menu::reset() {
    displayMenu = true;

    // Iterate over buttons by reference
    for (Button& button : aggregateButtons(buttons)) {
        button.reset();
    }
}
