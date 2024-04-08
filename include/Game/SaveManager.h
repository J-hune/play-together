#ifndef PLAY_TOGETHER_SAVEMANAGER_H
#define PLAY_TOGETHER_SAVEMANAGER_H

#include "Game.h"

class SaveManager {
public:

    /** CONSTRUCTOR **/

    explicit SaveManager(Game *game);


    /** MODIFIERS **/

    void setSlot(int value);


    /** METHODS **/

    /**
     * @brief Save the game state to a slot.
     */
    void saveGameState();

    /**
     * @brief Load the game state from a slot.
     * @return True if the game state was loaded successfully, false otherwise.
     */
    bool loadGameState();

private:
    Game *gamePtr; /**< A pointer to the game object. */
    int slot = 0; /**< The slot number to save the game state to. */
};

#endif //PLAY_TOGETHER_SAVEMANAGER_H
