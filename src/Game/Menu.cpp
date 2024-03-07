#include "../../include/Game/Menu.h"

/**
 * @brief Implementation of the Menu class responsible for rendering and handling events for the game menus.
 */


/** ACCESSORS **/

bool Menu::isDisplayingMenu() const {
    return displayMenu;
}

MenuAction Menu::getCurrentMenuAction() const {
    return currentMenuAction;
}


/** MODIFIERS **/

void Menu::setDisplayMenu(bool display_menu) {
    displayMenu = display_menu;
}

void Menu::setMenuAction(MenuAction menu_action) {
    currentMenuAction = menu_action;
}

void Menu::setQuit(bool quit_value) {
    *quit = quit_value;
}


/** FUNCTIONS **/

// Helper function to flatten a map of buttons into a vector of buttons
std::vector<Button> aggregateButtons(const std::map<GameStateKey, std::vector<Button>> &buttonsMap) {
    std::vector<Button> flattenedButtons;
    for (const auto &[state, buttons]: buttonsMap) {
        flattenedButtons.insert(flattenedButtons.end(), buttons.begin(), buttons.end());
    }
    return flattenedButtons;
}


/** METHODS **/

Menu::Menu(SDL_Renderer *renderer, TTF_Font *font, Game *game, bool *quit) : renderer(renderer), font(font), game(game), quit(quit) {
    // Create menu buttons
    SDL_Color normal_color = {100, 125, 160, 255};
    SDL_Color hover_color = {100, 105, 150, 255};
    SDL_Color text_color = {255, 255, 255, 255};

    SDL_Color quit_color = {255, 65, 55, 255};
    SDL_Color quit_hover_color = {255, 45, 35, 255};
    SDL_Color quit_text_color = {255, 255, 255, 255};

    // Add buttons to the main menu
    ButtonPosition play_button_position = {200, 60, 400, 100};
    ButtonPosition options_button_position = {200, 180, 400, 100};
    ButtonPosition credits_button_position = {200, 300, 400, 100};
    ButtonPosition quit_button_position = {200, 420, 400, 100};
    auto play_button = Button(renderer, font, play_button_position, "Jouer", ButtonAction::MENU_PLAY, normal_color, hover_color, text_color, 10);
    auto options_button = Button(renderer, font, options_button_position, "Options", ButtonAction::MENU_PLAY, normal_color, hover_color, text_color, 10);
    auto credits_button = Button(renderer, font, credits_button_position, "Credits", ButtonAction::MENU_PLAY, normal_color, hover_color, text_color, 10);
    auto quit_button = Button(renderer, font, quit_button_position, "Quitter", ButtonAction::QUIT, quit_color, quit_hover_color, quit_text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(play_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(options_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(credits_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(quit_button);


    // Add buttons to the play menu
    ButtonPosition start_new_game_button_position = {200, 60, 400, 100};
    ButtonPosition load_save_button_position = {200, 180, 400, 100};
    ButtonPosition main_menu_button_position = {200, 300, 400, 100};
    auto start_new_game_button = Button(renderer, font, start_new_game_button_position, "Start New Game", ButtonAction::START, normal_color, hover_color, text_color, 10);
    auto load_save_button = Button(renderer, font, load_save_button_position, "Load Save", ButtonAction::START, normal_color, hover_color, text_color, 10);
    auto main_menu_button = Button(renderer, font, main_menu_button_position, "Main Menu", ButtonAction::MENU_MAIN, normal_color, hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(start_new_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(load_save_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(main_menu_button);

    // Add a button to stop the game
    ButtonPosition resume_button_position = {200, 200, 400, 100};
    ButtonPosition stop_button_position = {200, 330, 400, 100};
    auto resume_button = Button(renderer, font, resume_button_position, "Resume", ButtonAction::RESUME, normal_color, hover_color, text_color, 10);
    auto stop_button = Button(renderer, font, stop_button_position, "Stop the Game!", ButtonAction::STOP, normal_color, hover_color, text_color, 10);

    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(resume_button);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(stop_button);
}

void Menu::render() {
    for (Button &button: buttons[{game->getGameState(), getCurrentMenuAction()}]) {
        button.render();
    }
}

void Menu::handleEvent(const SDL_Event &event) {
    // Get the buttons corresponding to the current game state
    auto &current_buttons = buttons[{game->getGameState(), getCurrentMenuAction()}];

    // Iterate through each button
    for (Button &button: current_buttons) {
        // Handle events for the button
        button.handleEvent(event);

        // Check if the button is clicked
        if (button.isButtonClicked()) {
            switch (button.getButtonAction()) {
                using enum ButtonAction;
                case START:
                    button.reset();
                    displayMenu = false;
                    setMenuAction(MenuAction::MAIN);
                    break;
                case RESUME:
                    displayMenu = false;
                    button.reset();
                    break;
                case STOP:
                    button.reset();
                    game->stop();
                    break;
                case MENU_MAIN:
                    setMenuAction(MenuAction::MAIN);
                    button.reset();
                    break;
                case MENU_PLAY:
                    setMenuAction(MenuAction::PLAY);
                    button.reset();
                    break;
                case MENU_LOAD_SAVE:
                    setMenuAction(MenuAction::LOAD_SAVE);
                    button.reset();
                    break;
                case MENU_START_NEW_GAME:
                    setMenuAction(MenuAction::START_NEW_GAME);
                    button.reset();
                    break;
                case QUIT:
                    setQuit(true);
                default:
                    break;
            }
        }
    }
}

void Menu::reset() {
    displayMenu = true;

    // Iterate over buttons by reference
    for (Button &button: aggregateButtons(buttons)) {
        button.reset();
    }
}
