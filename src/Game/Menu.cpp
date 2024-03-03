#include "../../include/Game/Menu.h"

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

Menu::Menu(SDL_Renderer *renderer, TTF_Font *font, Game *game, bool *quit) : renderer(renderer), font(font), game(game), quit(quit) {
    tcpClient.setDisconnectCallback([this] { onServerDisconnect(); });

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
    auto start_new_game_button = Button(renderer, font, start_new_game_button_position, "Start New Game", ButtonAction::START, normal_color, hover_color,text_color, 10);
    auto main_menu_button = Button(renderer, font, main_menu_button_position, "Main Menu", ButtonAction::NAVIGATE_TO_MENU_MAIN, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(host_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(join_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(start_new_game_button);
    buttons[{GameState::STOPPED, MenuAction::PLAY}].push_back(main_menu_button);


    // Add buttons to the host game menu
    ButtonPosition main_menu_button_position2 = {200, 420, 400, 100};
    ButtonPosition send_message_button_position = {200, 60, 400, 100};
    auto main_menu_button2 = Button(renderer, font, main_menu_button_position2, "Main Menu", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color,hover_color, text_color, 10);
    auto send_message_button = Button(renderer, font, send_message_button_position, "Send Message", ButtonAction::SEND_MESSAGE, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::HOST_GAME}].push_back(main_menu_button2);
    buttons[{GameState::STOPPED, MenuAction::HOST_GAME}].push_back(send_message_button);

    // Add buttons to the join game menu
    ButtonPosition main_menu_button_position3 = {200, 420, 400, 100};
    ButtonPosition send_message_button_position2 = {200, 60, 400, 100};
    auto main_menu_button3 = Button(renderer, font, main_menu_button_position3, "Main Précédent", ButtonAction::NAVIGATE_TO_MENU_PLAY, normal_color,hover_color, text_color, 10);
    auto send_message_button2 = Button(renderer, font, send_message_button_position2, "Send Message", ButtonAction::SEND_MESSAGE, normal_color,hover_color, text_color, 10);
    buttons[{GameState::STOPPED, MenuAction::JOIN_GAME}].push_back(main_menu_button3);
    buttons[{GameState::STOPPED, MenuAction::JOIN_GAME}].push_back(send_message_button2);


    // Add buttons to stop the game
    ButtonPosition resume_button_position = {200, 200, 400, 100};
    ButtonPosition stop_button_position = {200, 330, 400, 100};
    auto resume_button = Button(renderer, font, resume_button_position, "Resume", ButtonAction::RESUME, normal_color, hover_color,text_color, 10);
    auto stop_button = Button(renderer, font, stop_button_position, "Stop the Game!", ButtonAction::STOP, normal_color, hover_color,text_color, 10);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(resume_button);
    buttons[{GameState::PAUSED, MenuAction::MAIN}].push_back(stop_button);
}

Menu::~Menu() {
    // Stop the TCP server thread if it is running
    if (serverTCPThreadPtr && serverTCPThreadPtr->joinable()) {
        tcpServer.stop();
        serverTCPThreadPtr->request_stop();
        serverTCPThreadPtr->join();
    }

    // Stop the TCP client thread if it is running
    if (clientTCPThreadPtr && clientTCPThreadPtr->joinable()) {
        tcpClient.stop();
        clientTCPThreadPtr->request_stop();
        clientTCPThreadPtr->join();
    }

    // Stop the UDP server thread if it is running
    if (serverUDPThreadPtr && serverUDPThreadPtr->joinable()) {
        udpServer.stop();
        serverUDPThreadPtr->request_stop();
        serverUDPThreadPtr->join();
    }

    // Stop the UDP client thread if it is running
    if (clientUDPThreadPtr && clientUDPThreadPtr->joinable()) {
        udpClient.stop();
        clientUDPThreadPtr->request_stop();
        clientUDPThreadPtr->join();
    }
}

void Menu::render() {
    // Render buttons
    for (Button &button: buttons[{game->getGameState(), getCurrentMenuAction()}]) {
        button.render();
    }
}

bool Menu::isDisplayingMenu() const {
    return displayMenu;
}

MenuAction Menu::getCurrentMenuAction() const {
    return currentMenuAction;
}

void Menu::setDisplayMenu(bool display_menu) {
    displayMenu = display_menu;
}

void Menu::setMenuAction(MenuAction menu_action) {
    currentMenuAction = menu_action;
}

void Menu::setQuit(bool quit_value) {
    *quit = quit_value;
}

void Menu::reset() {
    displayMenu = true;

    // Reset all buttons
    for (Button &button: aggregateButtons(buttons)) {
        button.reset();
    }
}

void Menu::startServer() {
    if (tcpServer.initialize(8080)) {
        std::cout << "TCPServer: Server initialized and listening on port 8080" << std::endl;
        setMenuAction(MenuAction::HOST_GAME);

        // Start the server in a separate thread
        serverTCPThreadPtr = std::make_unique<std::jthread>(&TCPServer::start, &tcpServer);
    } else {
        std::cerr << "Failed to initialize TCPServer" << std::endl;
    }

    if (udpServer.initialize(8080)) {
        std::cout << "UDPServer: Server initialized and listening on port 8080" << std::endl;

        // Start the server in a separate thread
        serverUDPThreadPtr = std::make_unique<std::jthread>(&UDPServer::start, &udpServer);
    } else {
        std::cerr << "Failed to initialize UDPServer" << std::endl;
    }
}

void Menu::startClient() {
    if (tcpClient.connect("127.0.0.1", 8080)) {
        std::cout << "TCPClient: Connected to server" << std::endl;
        setMenuAction(MenuAction::JOIN_GAME);

        // Start the client in a separate thread
        clientTCPThreadPtr = std::make_unique<std::jthread>(&TCPClient::start, &tcpClient);
    } else {
        std::cerr << "Failed to connect TCPClient" << std::endl;
    }

    if (udpClient.initialize("127.0.0.1", 8080)) {
        // Start the client in a separate thread
        clientUDPThreadPtr = std::make_unique<std::jthread>(&UDPClient::start, &udpClient);
    } else {
        std::cerr << "Failed to connect UDPClient" << std::endl;
    }
}

void Menu::onServerDisconnect() {
    // Switch to MAIN menu on server disconnect
    tcpClient.stop();

    setMenuAction(MenuAction::MAIN);
    setDisplayMenu(true); // Make sure menu is displayed
}

void Menu::handleEvent(const SDL_Event &event) {
    auto &current_buttons = getCurrentMenuButtons();

    for (Button &button : current_buttons) {
        button.handleEvent(event);

        if (button.isButtonClicked()) {
            handleButtonAction(button);
        }
    }
}

std::vector<Button>& Menu::getCurrentMenuButtons() {
    return buttons[{game->getGameState(), getCurrentMenuAction()}];
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
    displayMenu = false;
    setMenuAction(MenuAction::MAIN);
}

void Menu::handleResumeButton(Button &button) {
    displayMenu = false;
    button.reset();
}

void Menu::handleStopButton(Button &button) {
    button.reset();
    game->stop();
}

void Menu::handleHostGameButton(Button &button) {
    startServer();
    button.reset();
}

void Menu::handleJoinGameButton(Button &button) {
    startClient();
    button.reset();
}

void Menu::handleNavigateToMainMenuButton(Button &button) {
    setMenuAction(MenuAction::MAIN);
    button.reset();
}

void Menu::handleNavigateToPlayMenuButton(Button &button) {
    setMenuAction(MenuAction::PLAY);
    button.reset();

    if (tcpServer.getSocketFileDescriptor() != -1) {
        tcpServer.stop();
        if (serverTCPThreadPtr && serverTCPThreadPtr->joinable()) {
            serverTCPThreadPtr->request_stop();
            serverTCPThreadPtr->join();
        }
        // Destroy the server thread
        serverTCPThreadPtr.reset();
    }

    if (tcpClient.getSocketFileDescriptor() != -1) {
        tcpClient.stop();
        if (clientTCPThreadPtr && clientTCPThreadPtr->joinable()) {
            clientTCPThreadPtr->request_stop();
            clientTCPThreadPtr->join();
        }
        // Destroy the client thread
        clientTCPThreadPtr.reset();
    }

    if (udpServer.getSocketFileDescriptor() != -1) {
        udpServer.stop();
        if (serverUDPThreadPtr && serverUDPThreadPtr->joinable()) {
            serverUDPThreadPtr->request_stop();
            serverUDPThreadPtr->join();
        }
        // Destroy the server thread
        serverUDPThreadPtr.reset();
    }

    if (udpClient.getSocketFileDescriptor() != -1) {
        udpClient.stop();
        if (clientUDPThreadPtr && clientUDPThreadPtr->joinable()) {
            clientUDPThreadPtr->request_stop();
            clientUDPThreadPtr->join();
        }
        // Destroy the client thread
        clientUDPThreadPtr.reset();
    }
}

void Menu::handleSendMessageButton(Button &button) {
    if (tcpClient.getSocketFileDescriptor() != -1) {
        tcpClient.send("Hello, World from TCP!");
    }

    if (udpClient.getSocketFileDescriptor() != -1) {
        udpClient.send("Hello, World from UDP!");
    }

    if (tcpServer.getSocketFileDescriptor() != -1) {
        bool success = tcpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }

    if (udpServer.getSocketFileDescriptor() != -1) {
        bool success = udpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }
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
    setQuit(true);
}
