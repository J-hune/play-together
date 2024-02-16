#ifndef GAME_CONSOLE_H
#define GAME_CONSOLE_H

#include <iostream>
#include <string>
#include <mutex>

#include "../../include/Game/Game.h"

/**
 * @brief The ApplicationConsole class represents a console for interacting with a Game.
 */
class ApplicationConsole {
public:
    /**
     * @brief Constructs a ApplicationConsole object.
     * @param game A pointer to the Game object that the console interacts with.
     */
    explicit ApplicationConsole(Game* game);

    /**
     * @brief Runs the console, allowing the user to input commands.
     */
    void Run() const;

    /**
     * @brief Executes a command entered by the user.
     * @param command The command to be executed.
     */
    void ExecuteCommand(const std::string& command) const;

    /**
     * @brief Requests the console to exit.
     */
    void RequestExit();

private:
    bool exitRequested = false;  /**< Flag indicating whether an exit has been requested. */
    std::mutex consoleMutex;  /**< Mutex for thread safety in the console. */
    Game* gamePtr;  /**< Pointer to the associated Game object. */
};

#endif // GAME_CONSOLE_H
