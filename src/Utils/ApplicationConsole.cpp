#include "../../include/Utils/ApplicationConsole.h"

// Constructor for ApplicationConsole, takes a pointer to the associated Game object.
ApplicationConsole::ApplicationConsole(Game* game) : gamePtr(game) {}

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

// Executes a command based on user input.
void ApplicationConsole::executeCommand(const std::string& command) const {

    // Commands that can be executed while the game is running
    if (gamePtr->getGameState() == GameState::RUNNING || gamePtr->getGameState() == GameState::PAUSED) {

        // Example of a simple command
        if (command == "help") {
            std::cout << "List of available commands: \n";
            std::cout << "help - Display help\n";
            std::cout << "exit - Exit the console\n";
            std::cout << "tp [x] [y] - Teleport the player to the specified coordinates\n";
        }

            // Teleport player to a specific location
        else if (command.find("tp") != std::string::npos) {
            float x;
            float y;
            if (sscanf(command.c_str(), "tp %f %f", &x, &y) == 2) {
                gamePtr->teleportPlayer(x, y);
            } else {
                std::cout << "Invalid syntax. Usage: tp [x] [y]\n";
            }
        }

            // Change map (load polygons from a map file)
        else if (command.find("map") != std::string::npos) {
            std::istringstream iss(command);
            std::string command_name;
            std::string map_name;
            iss >> command_name >> map_name;
            if (command_name == "map") {
                gamePtr->loadPolygonsFromMap(map_name);
            } else {
                std::cout << "Invalid syntax. Usage: map [mapName]\n";
            }
        }

        // Else display an error message
        else {
            std::cout << "Unknown command. Type 'help' to display help.\n";
        }
    }

    // Commands that can be executed while the game is not running
    else {
        if (command == "help") {
            std::cout << "List of available commands: \n";
            std::cout << "help - Display help\n";
            std::cout << "ping - Test the console\n";
        } else if (command == "ping") {
            std::cout << "Pong!\n";
        } else {
            std::cout << "Unknown command. Type 'help' to display help.\n";
        }
    }
}
