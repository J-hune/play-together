#include "../../include/Utils/ApplicationConsole.h"

/* CONSTRUCTORS */

ApplicationConsole::ApplicationConsole(Game *game) : gamePtr(game) {}


/* METHODS */

// Runs the console in a loop, waiting for user input and executing commands.
void ApplicationConsole::run() const {
    // Display a welcome message
    std::cout << "Welcome to the application console. Type 'help' to display available commands.\n";

    while (true) {
        std::string command;
        if (!std::getline(std::cin, command)) {
            break; // Break the loop if std::getline fails (e.g., if stdin is closed)
        }
        executeCommand(command);
    }

    std::cout << "Exiting console...\n";
}

void ApplicationConsole::executeGameRunningCommand(const std::string &command) const {
    if (command == "help") {
        displayHelp(1);
    } else if (command.find("tp") != std::string::npos) {
        teleportPlayer(command);
    } else if (command.find("map") != std::string::npos) {
        changeMap(command);
    } else if (command.find("show") != std::string::npos) {
        showDebugInfo(command);
    } else if (command.find("hide") != std::string::npos) {
        hideDebugInfo(command);
    } else if (command.find("enable") != std::string::npos) {
        enableMechanics(command);
    } else if (command.find("disable") != std::string::npos) {
        disableMechanics(command);
    } else if (command.find("render") != std::string::npos) {
        toggleRendering();
    } else if (command.find("fps") != std::string::npos) {
        toggleFPSRendering();
    } else {
        std::cout << "Unknown command. Type 'help' to display help.\n" << std::endl;
    }
}


void ApplicationConsole::executeGameNotRunningCommand(const std::string& command) const {
    if (command == "help") {
        displayHelp(0);
    } else if (command.find("fps") != std::string::npos) {
        changeMaxFrameRate(command);
    } else {
        std::cout << "Unknown command. Type 'help' to display help.\n" << std::endl;
    }
}



// Executes a command based on user input.
void ApplicationConsole::executeCommand(const std::string &command) const {
    GameState gameState = gamePtr->getGameState();

    // Commands that can be executed while the game is running
    if (gameState == GameState::RUNNING || gameState == GameState::PAUSED) {
        executeGameRunningCommand(command);
    } else {
        executeGameNotRunningCommand(command);
    }
}


void ApplicationConsole::displayHelp(int gameState) const {
    std::cout << "List of available commands: \n";
    std::cout << "help - Display help\n";

    if (gameState == 0) {
        std::cout << "fps - Toggle FPS display\n";
        std::cout << "tp [x] [y] - Teleport the player to the specified coordinates\n";
        std::cout << "show [all | camera_point | camera_area | player_colliders] - Show debug information\n";
        std::cout << "hide [all | camera_point | camera_area | player_colliders] - Hide debug information\n";
        std::cout << "enable [all | camera_shake | platforms] - Enable game mechanic\n";
        std::cout << "disable [all | camera_shake| platforms] - Disable game mechanic\n";
        std::cout << "render - Toggle rendering between textures and collisions box\n";
    } else {
        std::cout << "ping - Test the console\n";
        std::cout << "fps [fps] - Set the max frame rate (must be greater or equal to 30)\n";
    }
}


/* GAME RUNNING COMMANDS METHODS */

void ApplicationConsole::teleportPlayer(const std::string &command) const {
    float x; float y;
    if (sscanf(command.c_str(), "tp %f %f", &x, &y) == 2) {
        gamePtr->getPlayerManager().findPlayerById(-1)->teleportPlayer(x, y);
    } else {
        std::cout << "Invalid syntax. Usage: tp [x] [y]\n";
    }
}

void ApplicationConsole::changeMap(const std::string &command) const {
    std::istringstream iss(command);
    std::string command_name;
    std::string map_name;
    iss >> command_name >> map_name;
    if (command_name == "map") {
        gamePtr->setLevel(map_name);
    } else {
        std::cout << "Invalid syntax. Usage: map [mapName]\n";
    }
}

void ApplicationConsole::showDebugInfo(const std::string &command) const {
    RenderManager &renderManager = gamePtr->getRenderManager();
    std::istringstream iss(command);
    std::string command_name;
    std::string option;
    iss >> command_name >> option;

    if (command_name != "show") {
        std::cout << "Invalid syntax. Usage: show [all | camera_point | camera_area | player_colliders]\n";
        return;
    }

    if (option == "all") {
        renderManager.setRenderCameraPoint(true);
        renderManager.setRenderCameraArea(true);
        renderManager.setRenderPlayerColliders(true);
        std::cout << "Showing all information.\n";
    }
    else if (option == "camera_point") {
        renderManager.setRenderCameraPoint(true);
        std::cout << "Showing camera point information.\n";
    }
    else if (option == "camera_area") {
        renderManager.setRenderCameraArea(true);
        std::cout << "Showing camera area information.\n";
    }
    else if (option == "player_colliders") {
        renderManager.setRenderPlayerColliders(true);
        std::cout << "Showing player's colliders area information.\n";
    }
    else {
        std::cout << "Invalid option. Usage: show [all | camera_point | camera_area | player_colliders]\n";
    }
}

void ApplicationConsole::hideDebugInfo(const std::string &command) const {
    RenderManager &renderManager = gamePtr->getRenderManager();
    std::istringstream iss(command);
    std::string command_name;
    std::string option;
    iss >> command_name >> option;

    if (command_name != "hide") {
        std::cout << "Invalid syntax. Usage: hide [all | camera_point | camera_area | player_colliders]\n";
        return;
    }

    if (option == "all") {
        renderManager.setRenderCameraPoint(false);
        renderManager.setRenderCameraArea(false);
        renderManager.setRenderPlayerColliders(false);
        std::cout << "Hiding all information.\n";
    }
    else if (option == "camera_point") {
        renderManager.setRenderCameraPoint(false);
        std::cout << "Hiding camera point information.\n";
    }
    else if (option == "camera_area") {
        renderManager.setRenderCameraArea(false);
        std::cout << "Hiding camera area information.\n";
    }
    else if (option == "player_colliders") {
        renderManager.setRenderPlayerColliders(false);
        std::cout << "Hiding player's colliders information.\n";
    }
    else {
        std::cout << "Invalid option. Usage: hide [all | camera_point | camera_area | player_colliders]\n";
    }
}

void ApplicationConsole::enableMechanics(const std::string &command) const {
    std::istringstream iss(command);
    std::string command_name;
    std::string option;
    iss >> command_name >> option;

    if (command_name != "enable") {
        std::cout << "Invalid syntax. Usage: enable [all | camera_shake | platforms]\n";
        return;
    }

    if (option == "all") {
        gamePtr->getCamera()->setShake(-1);
        gamePtr->setEnablePlatformsMovement(true);
        std::cout << "Enabling all mechanics.\n";
    }
    else if (option == "camera_shake") {
        gamePtr->getCamera()->setShake(-1);
        std::cout << "Enabling camera shaking.\n";
    }
    else if (option == "platforms") {
        gamePtr->setEnablePlatformsMovement(true);
        std::cout << "Enabling platforms movement.\n";
    }
    else {
        std::cout << "Invalid option. Usage: enable [all | camera_shake | platforms]\n";
    }
}

void ApplicationConsole::disableMechanics(const std::string &command) const {
    std::istringstream iss(command);
    std::string command_name;
    std::string option;
    iss >> command_name >> option;

    if (command_name != "disable") {
        std::cout << "Invalid syntax. Usage: disable [all | camera_shake | platforms]\n";
        return;
    }

    if (option == "all") {
        gamePtr->getCamera()->setShake(0);
        gamePtr->setEnablePlatformsMovement(false);
        std::cout << "Disabling all mechanics.\n";
    }
    else if (option == "camera_shake") {
        gamePtr->getCamera()->setShake(0);
        std::cout << "Disabling camera shaking.\n";
    }
    else if (option == "platforms") {
        gamePtr->setEnablePlatformsMovement(false);
        std::cout << "Disabling platforms' movement.\n";
    }
    else {
        std::cout << "Invalid option. Usage: disable [all | camera_shake | platforms]\n";
    }
}

void ApplicationConsole::toggleRendering() const {
    gamePtr->getRenderManager().toggleRenderTextures();
    std::cout << "RenderManager toggled.\n";
}

void ApplicationConsole::toggleFPSRendering() const {
    gamePtr->getRenderManager().toggleRenderFps();
    std::cout << "FPS rendering toggled.\n";
}


/* GAME NOT RUNNING COMMANDS METHODS */

void ApplicationConsole::changeMaxFrameRate(const std::string& command) const {
    int fps;
    if (sscanf(command.c_str(), "fps %d", &fps) != 1) {
        std::cout << "Invalid syntax. Usage: fps [fps]\n";
        return;
    }

    if (fps < gamePtr->getTickRate()) {
        std::cout << "Invalid frame rate. Frame rate must be greater or equal to 30.\n";
        return;
    }

    gamePtr->setFrameRate(fps);
    std::cout << "Max frame rate set to " << fps << ".\n";
}

