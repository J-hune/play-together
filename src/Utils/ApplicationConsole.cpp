#include "../../include/Utils/ApplicationConsole.h"

// Constructor for ApplicationConsole, takes a pointer to the associated Game object.
ApplicationConsole::ApplicationConsole(Game* game) : gamePtr(game) {}

// Runs the console in a loop, waiting for user input and executing commands.
void ApplicationConsole::Run() const {
    // Display a welcome message
    std::cout << "Welcome to the application console. Type 'help' to display available commands.\n";

    while (!exitRequested) {
        std::string command;
        std::getline(std::cin, command);
        ExecuteCommand(command);
    }
}

// Executes a command based on user input.
void ApplicationConsole::ExecuteCommand(const std::string& command) const {

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
            int x;
            int y;
            if (sscanf(command.c_str(), "tp %d %d", &x, &y) == 2) {
                gamePtr->teleportPlayer(x, y);
            } else {
                std::cout << "Invalid syntax. Usage: tp [x] [y]\n";
            }
        }

            // Change map (load polygons from a map file)
        else if (command.find("map") != std::string::npos) {
            std::istringstream iss(command);
            std::string commandName;
            std::string mapName;
            iss >> commandName >> mapName;
            if (commandName == "map") {
                gamePtr->loadPolygonsFromMap(mapName);
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

// Requests the console to exit by setting the exitRequested flag.
void ApplicationConsole::RequestExit() {
    exitRequested = true;
}
