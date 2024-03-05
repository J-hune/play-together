#ifndef GAME_CONSOLE_H
#define GAME_CONSOLE_H

#include <iostream>
#include <string>
#include <mutex>

#include "../../include/Game/Game.h"

/**
 * @Class Application
    * @brief The ApplicationConsole class represent an interface for the developer to interact with the game.
 */
class ApplicationConsole {
public:
    /** CONSTRUCTOR **/

    /**
     * @brief Constructs a ApplicationConsole object.
     * @param game A pointer to the Game object that the console interacts with.
     */
    explicit ApplicationConsole(Game* game);


    /** PUBLIC METHODS **/

    /**
     * @brief Runs the console, allowing the user to input commands.
     */
    void run() const;

    /**
     * @brief Executes a command entered by the user.
     * @param command The command to be executed.
     */
    void executeCommand(const std::string& command) const;

private:
    /** ATTRIBUTES **/

    std::mutex consoleMutex;  /**< Mutex for thread safety in the console. */
    Game* gamePtr;  /**< Pointer to the associated Game object. */
};

#endif // GAME_CONSOLE_H
