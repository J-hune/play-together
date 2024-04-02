#include "../../include/Utils/ApplicationConsole.h"

/** CONSTRUCTOR **/

// Constructor for ApplicationConsole, takes a pointer to the associated Game object.
ApplicationConsole::ApplicationConsole(Game* game) : gamePtr(game) {}


/** METHODS **/

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
            std::cout << "fps - Toggle FPS display\n";
            std::cout << "tp [x] [y] - Teleport the player to the specified coordinates\n";
            std::cout << "show [all | camera_point | camera_area | player_colliders] - Show debug information\n";
            std::cout << "hide [all | camera_point | camera_area | player_colliders] - Hide debug information\n";
            std::cout << "enable [all | camera_shake | platforms] - Enable game mechanic\n";
            std::cout << "disable [all | camera_shake| platforms] - Disable game mechanic\n";
            std::cout << "render - Toggle rendering between textures and collisions box\n";
        }

        // Teleport player to a specific location
        else if (command.find("tp") != std::string::npos) {
            float x;
            float y;
            if (sscanf(command.c_str(), "tp %f %f", &x, &y) == 2) {
                gamePtr->findPlayerById(-1)->teleportPlayer(x, y);
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
                gamePtr->setLevel(map_name);
            } else {
                std::cout << "Invalid syntax. Usage: map [mapName]\n";
            }
        }

        // Show debug information
        else if (command.find("show") != std::string::npos) {
            std::istringstream iss(command);
            std::string command_name;
            std::string option;
            iss >> command_name >> option;
            if (command_name == "show") {
                if (option == "all") {
                    gamePtr->setRenderCameraPoint(true);
                    gamePtr->setRenderCameraArea(true);
                    gamePtr->setRenderPlayerColliders(true);
                    std::cout << "Showing all information.\n";
                }
                else if (option == "camera_point") {
                    gamePtr->setRenderCameraPoint(true);
                    std::cout << "Showing camera point information.\n";
                } else if (option == "camera_area") {
                    gamePtr->setRenderCameraArea(true);
                    std::cout << "Showing camera area information.\n";
                } else if (option == "player_colliders") {
                    gamePtr->setRenderPlayerColliders(true);
                    std::cout << "Showing player's colliders area information.\n";
                } else {
                    std::cout << "Invalid option. Usage: show [all | camera_point | camera_area | player_colliders]\n";
                }
            } else {
                std::cout << "Invalid syntax. Usage: show [all | camera_point | camera_area | player_colliders]\n";
            }
        }

        // Hide debug information
        else if (command.find("hide") != std::string::npos) {
            std::istringstream iss(command);
            std::string command_name;
            std::string option;
            iss >> command_name >> option;
            if (command_name == "hide") {
                if (option == "all") {
                    gamePtr->setRenderCameraPoint(false);
                    gamePtr->setRenderCameraArea(false);
                    gamePtr->setRenderPlayerColliders(false);
                    std::cout << "Hiding all information.\n";
                }
                else if (option == "camera_point") {
                    gamePtr->setRenderCameraPoint(false);
                    std::cout << "Hiding camera point information.\n";
                } else if (option == "camera_area") {
                    gamePtr->setRenderCameraArea(false);
                    std::cout << "Hiding camera area information.\n";
                } else if (option == "player_colliders") {
                    gamePtr->setRenderPlayerColliders(false);
                    std::cout << "Hiding player's colliders information.\n";
                } else {
                    std::cout << "Invalid option. Usage: hide [all | camera_point | camera_area | player_colliders]\n";
                }
            } else {
                std::cout << "Invalid syntax. Usage: hide [all | camera_point | camera_area | player_colliders]\n";
            }
        }

        // Enable game mechanics
        else if (command.find("enable") != std::string::npos) {
            std::istringstream iss(command);
            std::string command_name;
            std::string option;
            iss >> command_name >> option;
            if (command_name == "enable") {
                if (option == "all") {
                    gamePtr->getCamera()->setIsShaking(true);
                    gamePtr->setEnablePlatformsMovement(true);
                    std::cout << "Enabling all mechanics.\n";
                }
                else if (option == "camera_shake") {
                    gamePtr->getCamera()->setIsShaking(true);
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
            else {
                std::cout << "Invalid syntax. Usage: enable [all | camera_shake | platforms]\n";
            }
        }

        // Disable game mechanics
        else if (command.find("disable") != std::string::npos) {
            std::istringstream iss(command);
            std::string command_name;
            std::string option;
            iss >> command_name >> option;
            if (command_name == "disable") {
                if (option == "all") {
                    gamePtr->getCamera()->setIsShaking(false);
                    gamePtr->setEnablePlatformsMovement(false);
                    std::cout << "Disabling all mechanics.\n";
                }
                else if (option == "camera_shake") {
                    gamePtr->getCamera()->setIsShaking(false);
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
            else {
                std::cout << "Invalid syntax. Usage: disable [all | camera_shake | platforms]\n";
            }
        }

        // Toggle rendering
        else if (command.find("render") != std::string::npos) {
            gamePtr->toggleRenderTextures();
            std::cout << "Rendering toggled.\n";
        }

        // Toggle FPS rendering
        else if (command.find("fps") != std::string::npos) {
            gamePtr->toggleRenderFps();
            std::cout << "FPS rendering toggled.\n";
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
        }

        // change the max frame rate
        else if (command.find("fps") != std::string::npos) {
            int fps;
            if (sscanf(command.c_str(), "fps %d", &fps) == 1) {
                gamePtr->setFrameRate(fps);
                std::cout << "Max frame rate set to " << fps << ".\n";
            } else {
                std::cout << "Invalid syntax. Usage: fps [fps]\n";
            }
        }

        else if (command == "ping") {
            std::cout << "Pong!\n";
        }

        else {
            std::cout << "Unknown command. Type 'help' to display help.\n";
        }
    }
}
