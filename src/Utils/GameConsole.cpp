#include "../../include/Utils/GameConsole.h"

// Constructor for GameConsole, takes a pointer to the associated Game object.
GameConsole::GameConsole(Game* game) : gamePtr(game) {}

// Runs the console in a loop, waiting for user input and executing commands.
void GameConsole::Run() const {
    while (!exitRequested) {
        std::string command;
        std::getline(std::cin, command);
        ExecuteCommand(command);
    }
}

// Executes a command based on user input.
void GameConsole::ExecuteCommand(const std::string& command) const {

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

// Requests the console to exit by setting the exitRequested flag.
void GameConsole::RequestExit() {
    exitRequested = true;
}
