#include "../../include/Game/Menu.h"
#include "../../include/Network/TCPError.h"
#include "../../include/Network/UDPError.h"

/** FUNCTIONS **/

/**
 * @brief Implementation of the Menu class responsible for rendering and handling events for the game menus.
 */

// Helper function to flatten a map of buttons into a vector of buttons
std::vector<Button> aggregateButtons(const std::map<GameStateKey, std::vector<Button>> &buttonsMap) {
    std::vector<Button> flattenedButtons;
    for (const auto &[state, buttons]: buttonsMap) {
        flattenedButtons.insert(flattenedButtons.end(), buttons.begin(), buttons.end());
    }
    return flattenedButtons;
}

/** CONSTRUCTOR **/

Menu::Menu(SDL_Renderer *renderer, TTF_Font *font, bool *quit) : renderer(renderer), fontPtr(font), quitPtr(quit) {
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
    auto play_button = Button(renderer, font, play_button_position, "Jouer", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color, hover_color, text_color,10);
    auto options_button = Button(renderer, font, options_button_position, "Options", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color, hover_color,text_color, 10);
    auto credits_button = Button(renderer, font, credits_button_position, "Credits", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color, hover_color,text_color, 10);
    auto quit_button = Button(renderer, font, quit_button_position, "Quitter", ButtonAction::QUIT, quit_color, quit_hover_color,quit_text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(play_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(options_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(credits_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(quit_button);


    // Add buttons to the play menu
    ButtonPosition host_game_button_position = {200, 60, 400, 100};
    ButtonPosition join_game_button_position = {200, 180, 400, 100};
    ButtonPosition start_new_game_button_position = {200, 300, 400, 100};
    ButtonPosition main_menu_button_position = {200, 420, 400, 100};
    auto host_game_button = Button(renderer, font, host_game_button_position, "Host Game", ButtonAction::HOST_GAME, normal_color,hover_color, text_color, 10);
    auto join_game_button = Button(renderer, font, join_game_button_position, "Join Hosted Game", ButtonAction::JOIN_GAME, normal_color,hover_color, text_color, 10);
    auto start_new_game_button = Button(renderer, font, start_new_game_button_position, "Start Local Game", ButtonAction::START, normal_color, hover_color,text_color, 10);
    auto main_menu_button = Button(renderer, font, main_menu_button_position, "Main Menu", ButtonAction::NAVIGATE_TO_MENU_MAIN, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(host_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(join_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(start_new_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(main_menu_button);


    // Add buttons to the host game menu
    ButtonPosition send_message_button_position = {200, 60, 400, 100};
    ButtonPosition view_game_button_position = {200, 180, 400, 100};
    ButtonPosition main_menu_button_position2 = {200, 420, 400, 100};
    auto send_message_button = Button(renderer, font, send_message_button_position, "Send Message", ButtonAction::SEND_MESSAGE, normal_color,hover_color, text_color, 10);
    auto view_game_button = Button(renderer, font, view_game_button_position, "View Game", ButtonAction::START, normal_color,hover_color, text_color, 10);
    auto main_menu_button2 = Button(renderer, font, main_menu_button_position2, "Main Menu", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::HOST_GAME}].push_back(send_message_button);
    buttons[{GameState::STOPPED, MenuAction::HOST_GAME}].push_back(view_game_button);
    buttons[{GameState::STOPPED, MenuAction::HOST_GAME}].push_back(main_menu_button2);

    // Add buttons to the join game menu
    ButtonPosition send_message_button_position2 = {200, 60, 400, 100};
    ButtonPosition view_game_button_position2 = {200, 180, 400, 100};
    ButtonPosition main_menu_button_position3 = {200, 420, 400, 100};
    auto send_message_button2 = Button(renderer, font, send_message_button_position2, "Send Message", ButtonAction::SEND_MESSAGE, normal_color,hover_color, text_color, 10);
    auto view_game_button2 = Button(renderer, font, view_game_button_position2, "View Game", ButtonAction::START, normal_color,hover_color, text_color, 10);
    auto main_menu_button3 = Button(renderer, font, main_menu_button_position3, "Main Précédent", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::JOIN_GAME}].push_back(send_message_button2);
    buttons[{GameState::STOPPED, MenuAction::JOIN_GAME}].push_back(view_game_button2);
    buttons[{GameState::STOPPED, MenuAction::JOIN_GAME}].push_back(main_menu_button3);


    // Add buttons to stop the game
    ButtonPosition resume_button_position = {200, 200, 400, 100};
    ButtonPosition stop_button_position = {200, 330, 400, 100};
    auto resume_button = Button(renderer, font, resume_button_position, "Resume", ButtonAction::RESUME, normal_color, hover_color,text_color, 10);
    auto stop_button = Button(renderer, font, stop_button_position, "Stop the Game!", ButtonAction::STOP, normal_color, hover_color,text_color, 10);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(resume_button);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(stop_button);
}

/** ACCESSORS **/

bool Menu::isDisplayingMenu() const {
    return displayMenu;
}

MenuAction Menu::getCurrentMenuAction() const {
    return currentMenuAction;
}

std::vector<Button> &Menu::getCurrentMenuButtons() {
    return buttons[{Mediator::getGameState(), getCurrentMenuAction()}];
}


/** MODIFIERS **/

void Menu::setDisplayMenu(bool display_menu) {
    displayMenu = display_menu;
}

void Menu::setMenuAction(MenuAction menu_action) {
    currentMenuAction = menu_action;
}

void Menu::setQuit(bool quit_value) {
    *quitPtr = quit_value;
}


/** METHODS **/

void Menu::render() {
    // Render buttons
    for (Button &button: buttons[{Mediator::getGameState(), getCurrentMenuAction()}]) {
        button.render();
    }
}

void Menu::reset() {
    displayMenu = true;

    // Reset all buttons
    for (Button &button: aggregateButtons(buttons)) {
        button.reset();
    }
}

void Menu::onServerDisconnect() {
    // Switch to MAIN menu on server disconnect
    Mediator::stop();
    setMenuAction(MenuAction::MAIN);
    setDisplayMenu(true); // Make sure menu is displayed
}

void Menu::handleEvent(const SDL_Event &event) {
    auto &current_buttons = getCurrentMenuButtons();

    for (Button &button: current_buttons) {
        button.handleEvent(event);

        if (button.isButtonClicked()) {
            handleButtonAction(button);
        }
    }
}

void Menu::handleButtonAction(Button &button) {
    switch (button.getButtonAction()) {
        using enum ButtonAction;
        case START:
            handleStartButton(button);
            break;
        case RESUME:
            handleResumeButton(button);
            break;
        case STOP:
            handleStopButton(button);
            break;
        case HOST_GAME:
            handleHostGameButton(button);
            break;
        case JOIN_GAME:
            handleJoinGameButton(button);
            break;
        case NAVIGATE_TO_MENU_MAIN:
            handleNavigateToMainMenuButton(button);
            break;
        case NAVIGATE_TO_MENU_PLAY:
            handleNavigateToPlayMenuButton(button);
            break;
        case SEND_MESSAGE:
            handleSendMessageButton(button);
            break;
        case NAVIGATE_TO_MENU_LOAD_SAVE:
            handleNavigateToLoadSaveMenuButton(button);
            break;
        case NAVIGATE_TO_MENU_START_NEW_GAME:
            handleNavigateToStartNewGameMenuButton(button);
            break;
        case QUIT:
            handleQuitButton(button);
            break;
        default:
            break;
    }
}

void Menu::handleStartButton(Button &button) {
    button.reset();
    Mediator::initializeGame();
    displayMenu = false;
    setMenuAction(MenuAction::MAIN);
}

void Menu::handleResumeButton(Button &button) {
    displayMenu = false;
    button.reset();
}

void Menu::handleStopButton(Button &button) {
    button.reset();
    Mediator::stop();
    setDisplayMenu(true);

    Mediator::stopServers();
    Mediator::stopClients();
}

void Menu::handleHostGameButton(Button &button) {
    Mediator::stopClients();

    try {
        Mediator::startServers();
        setMenuAction(MenuAction::HOST_GAME);
    } catch (const TCPError& e) {
        std::cerr << "(TCPError) " << e.what() << std::endl;
    } catch (const UDPError& e) {
        std::cerr << "(UDPError) " << e.what() << std::endl;
    }

    button.reset();
}

void Menu::handleJoinGameButton(Button &button) {
    Mediator::stopServers();

    try {
        Mediator::startClients();
        setMenuAction(MenuAction::JOIN_GAME);
    } catch (const TCPError& e) {
        std::cerr << "(TCPError) " << e.what() << std::endl;
    } catch (const UDPError& e) {
        std::cerr << "(UDPError) " << e.what() << std::endl;
    }

    button.reset();
}

void Menu::handleNavigateToMainMenuButton(Button &button) {
    setMenuAction(MenuAction::MAIN);
    button.reset();
}

void Menu::handleNavigateToPlayMenuButton(Button &button) {
    setMenuAction(MenuAction::PLAY);
    button.reset();

    Mediator::stopServers();
    Mediator::stopClients();
}

void Menu::handleSendMessageButton(Button &button) const {
    Mediator::temporarySendMethod("Hello, World!");
    button.reset();
}

void Menu::handleNavigateToLoadSaveMenuButton(Button &button) {
    setMenuAction(MenuAction::LOAD_SAVE);
    button.reset();
}

void Menu::handleNavigateToStartNewGameMenuButton(Button &button) {
    setMenuAction(MenuAction::START_NEW_GAME);
    button.reset();
}

void Menu::handleQuitButton([[maybe_unused]] Button &button) {
    button.reset();
    setQuit(true);
}