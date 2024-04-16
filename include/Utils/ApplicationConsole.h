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
private:
    /* ATTRIBUTES */

    Game *gamePtr;  /**< Pointer to the associated Game object. */


public:
    /* CONSTRUCTOR */

    /**
     * @brief Constructs a ApplicationConsole object.
     * @param game A pointer to the Game object that the console interacts with.
     */
    explicit ApplicationConsole(Game *game);


    /* PUBLIC METHODS */

    /**
     * @brief Runs the console, allowing the user to input commands.
     */
    void run() const;

    /**
     * @brief Executes a command entered by the user.
     * @param command The command to be executed.
     */
    void executeCommand(const std::string &command) const;

private:

    /**
     * @brief Executes a command when the game is running.
     * @param command The command to be executed.
     */
    void executeGameRunningCommand(const std::string &command) const;

    /**
     * @brief Executes a command when the game is not running.
     * @param command The command to be executed.
     */
    void executeGameNotRunningCommand(const std::string &command) const;


    /* COMMAND METHODS */

    void displayHelp(int gameState) const;
    void teleportPlayer(const std::string& command) const;
    void changeMap(const std::string& command) const;
    void showDebugInfo(const std::string& command) const;
    void hideDebugInfo(const std::string& command) const;
    void enableMechanics(const std::string& command) const;
    void disableMechanics(const std::string& command) const;
    void toggleRendering() const;
    void toggleFPSRendering() const;
    void changeMaxFrameRate(const std::string& command) const;
};

#endif // GAME_CONSOLE_H
