#ifndef PLAY_TOGETHER_INPUTMANAGER_H
#define PLAY_TOGETHER_INPUTMANAGER_H

#include <SDL_events.h>
#include "../Player.h"
#include "../Game.h"

/**
 * @file InputManager.h
 * @brief Defines the InputManager class responsible for handling the input events.
 */


class InputManager {
private:
    /* ATTRIBUTES */

    Game *gamePtr; /**< A pointer to the game object. */


public:

    /* CONSTRUCTORS */

    explicit InputManager(Game *game);


    /* METHODS */

    void handleKeyboardEvents();

    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent &keyEvent);

    void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const;

private:

    /**
     * @brief Sends the keyboard state to the network.
     * @param lastKeyboardStateMask The last keyboard state mask.
     */
    void sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMask) const;

};

#endif //PLAY_TOGETHER_INPUTMANAGER_H
