#ifndef PLAY_TOGETHER_SAVEMANAGER_H
#define PLAY_TOGETHER_SAVEMANAGER_H

#include <format>
#include "../Game.h"

/**
 * @file SaveManager.h
 * @brief Defines the SaveManager class responsible for saving and loading the game state.
 */


class SaveManager {
private:
    /* ATTRIBUTES */

    Game *gamePtr; /**< A pointer to the game object. */
    int slot = 0; /**< The slot number to save the game state to. */


public:

    /* CONSTRUCTOR */

    explicit SaveManager(Game *game);


    /* MODIFIERS */

    void setSlot(int value);


    /* METHODS */

    /**
     * @brief Save the game state to a slot.
     */
    void saveGameState();

    /**
     * @brief Load the game state from a slot.
     * @return True if the game state was loaded successfully, false otherwise.
     */
    bool loadGameState();

};

#endif //PLAY_TOGETHER_SAVEMANAGER_H
