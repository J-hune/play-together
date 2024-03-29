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

    SDL_Color disabled_color = {135, 135, 135, 255};
    SDL_Color disabled_hover_color = {135, 135, 135, 255};

    // Add buttons to the main menu
    ButtonPosition play_button_position = {200, 60, 400, 100};
    ButtonPosition options_button_position = {200, 180, 400, 100};
    ButtonPosition credits_button_position = {200, 300, 400, 100};
    ButtonPosition quit_button_position = {200, 420, 400, 100};
    auto play_button = Button(renderer, font, play_button_position, 0, "Jouer", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color, hover_color, text_color,10);
    auto options_button = Button(renderer, font, options_button_position, 0, "Options", ButtonAction::NONE, normal_color, hover_color,text_color, 10);
    auto credits_button = Button(renderer, font, credits_button_position, 0, "Credits", ButtonAction::NONE, normal_color, hover_color,text_color, 10);
    auto quit_button = Button(renderer, font, quit_button_position, 0, "Quitter", ButtonAction::QUIT, quit_color, quit_hover_color,quit_text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(play_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(options_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(credits_button);
    buttons[{GameState::STOPPED, MenuAction::MAIN}].push_back(quit_button);


    // Add buttons to the play menu
    ButtonPosition host_game_button_position = {200, 60, 400, 100};
    ButtonPosition join_game_button_position = {200, 180, 400, 100};
    ButtonPosition start_new_game_button_position = {200, 300, 400, 100};
    ButtonPosition main_menu_button_position = {200, 420, 400, 100};
    auto host_game_button = Button(renderer, font, host_game_button_position, 1, "Host Game", ButtonAction::CREATE_OR_LOAD_GAME, normal_color,hover_color, text_color, 10);
    auto join_game_button = Button(renderer, font, join_game_button_position, 0, "Join Hosted Game", ButtonAction::JOIN_HOSTED_GAME, normal_color,hover_color, text_color, 10);
    auto start_new_game_button = Button(renderer, font, start_new_game_button_position, 0, "Start Local Game", ButtonAction::CREATE_OR_LOAD_GAME, normal_color, hover_color,text_color, 10);
    auto main_menu_button = Button(renderer, font, main_menu_button_position, 0, "Main Menu",  ButtonAction::NAVIGATE_TO_MENU_MAIN, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(host_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(join_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(start_new_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(main_menu_button);


    // Add buttons to paused menu
    ButtonPosition resume_button_position = {200, 100, 400, 100};
    ButtonPosition save_button_position = {200, 220, 400, 100};
    ButtonPosition stop_button_position = {200, 340, 400, 100};
    auto resume_button = Button(renderer, font, resume_button_position, 0, "Resume", ButtonAction::RESUME, normal_color, hover_color,text_color, 10);
    auto save_button = Button(renderer, font, save_button_position, 0, "Save the Game", ButtonAction::SAVE, normal_color, hover_color,text_color, 10);
    auto stop_button = Button(renderer, font, stop_button_position, 0, "Stop the Game", ButtonAction::STOP, normal_color, hover_color,text_color, 10);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(resume_button);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(save_button);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(stop_button);

    // Add buttons to the create or load game menu
    ButtonPosition save_slot1_button_position = {200, 80, 300, 80};
    ButtonPosition remove_slot1_button_position = {510, 80, 90, 80};
    ButtonPosition save_slot2_button_position = {200, 180, 300, 80};
    ButtonPosition remove_slot2_button_position = {510, 180, 90, 80};
    ButtonPosition save_slot3_button_position = {200, 280, 300, 80};
    ButtonPosition remove_slot3_button_position = {510, 280, 90, 80};
    ButtonPosition main_menu_button_position4 = {200, 400, 400, 100};
    auto save_slot1_button = Button(renderer, font, save_slot1_button_position, 1, "Empty Slot", ButtonAction::VIEW_GAME, normal_color, hover_color,text_color, 10);
    auto remove_slot1_button = Button(renderer, font, remove_slot1_button_position, 1, "R", ButtonAction::DELETE_SAVE, disabled_color, disabled_hover_color,text_color, 10);
    auto save_slot2_button = Button(renderer, font, save_slot2_button_position, 2, "Empty Slot", ButtonAction::VIEW_GAME, normal_color, hover_color,text_color, 10);
    auto remove_slot2_button = Button(renderer, font, remove_slot2_button_position, 2, "R", ButtonAction::DELETE_SAVE, disabled_color, disabled_hover_color,text_color, 10);
    auto save_slot3_button = Button(renderer, font, save_slot3_button_position, 3, "Empty Slot", ButtonAction::VIEW_GAME, normal_color, hover_color,text_color, 10);
    auto remove_slot3_button = Button(renderer, font, remove_slot3_button_position, 3, "R", ButtonAction::DELETE_SAVE, disabled_color, disabled_hover_color,text_color, 10);
    auto main_menu_button4 = Button(renderer, font, main_menu_button_position4, 0, "Main Menu", ButtonAction::NAVIGATE_TO_MENU_MAIN, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(save_slot1_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(remove_slot1_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(save_slot2_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(remove_slot2_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(save_slot3_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(remove_slot3_button);
    buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}].push_back(main_menu_button4);
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
        case VIEW_GAME:
            handleStartButton(button, button.getValue());
            break;
        case RESUME:
            handleResumeButton(button);
            break;
        case STOP:
            handleStopButton(button);
            break;
        case SAVE:
            handleSaveButton(button);
            break;
        case QUIT:
            handleQuitButton(button);
            break;
        case CREATE_OR_LOAD_GAME:
            handleCreateOrLoadGameButton(button);
            break;
        case DELETE_SAVE:
            handleDeleteSaveButton(button);
            break;
        case JOIN_HOSTED_GAME:
            handleJoinHostedGameButton(button);
            break;
        case NAVIGATE_TO_MENU_MAIN:
            handleNavigateToMainMenuButton(button);
            break;
        case NAVIGATE_TO_MENU_PLAY:
            handleNavigateToPlayMenuButton(button);
            break;
        default:
            break;
    }
}

void Menu::handleStartButton(Button &button, int slot) {
    button.reset();
    Mediator::initializeGame(slot);
    displayMenu = false;
    setMenuAction(MenuAction::MAIN);
}

void Menu::handleResumeButton(Button &button) {
    displayMenu = false;
    button.reset();
}

void Menu::handleSaveButton(Button &button) {
    button.reset();
    Mediator::save();
}

void Menu::handleStopButton(Button &button) {
    button.reset();
    Mediator::stop();
    setDisplayMenu(true);

    Mediator::stopServers();
    Mediator::stopClients();
}

void Menu::handleCreateOrLoadGameButton(Button &button) {
    updateSaveSlots();

    if (button.getValue() == 1) {
        gameMode = 1;
        Mediator::stopClients();

        try {
            Mediator::startServers();
        } catch (const TCPError& e) {
            std::cerr << "(TCPError) " << e.what() << std::endl;
        } catch (const UDPError& e) {
            std::cerr << "(UDPError) " << e.what() << std::endl;
        }
    } else {
        gameMode = 0;
    }

    setMenuAction(MenuAction::CREATE_OR_LOAD_GAME);
    button.reset();
}

void Menu::handleJoinHostedGameButton(Button &button) {
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

void Menu::handleDeleteSaveButton(Button &button) {
    // Delete the save file and update the save slots
    button.reset();

    std::string save_file = "saves/slot_" + std::to_string(button.getValue()) + ".json";
    std::remove(save_file.c_str());
    updateSaveSlots();
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

void Menu::handleQuitButton([[maybe_unused]] Button &button) {
    button.reset();
    setQuit(true);
}

void Menu::updateSaveSlots() {

    // Update the text for the save slots
    for (int i = 0; i < 3; i++) {
        std::ifstream save_slot("saves/slot_" + std::to_string(i + 1) + ".json");
        Button &button = buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}][i * 2];
        Button &remove_button = buttons[{GameState::STOPPED, MenuAction::CREATE_OR_LOAD_GAME}][i * 2 + 1];

        if (save_slot.good()) {
            using json = nlohmann::json;
            json save_slot_json;
            save_slot >> save_slot_json;

            button.setButtonText(save_slot_json["date"].get<std::string>() + " (" + save_slot_json["level"].get<std::string>() + ")");
            remove_button.setNormalColor({255, 65, 55, 255});
            remove_button.setHoverColor({255, 45, 35, 255});
            save_slot.close();
        } else {
            button.setButtonText("Empty Slot");
            remove_button.setNormalColor({135, 135, 135, 255});
            remove_button.setHoverColor({135, 135, 135, 255});
        }
    }
}