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
    uint16_t lastKeyboardStateMask = 0; /**< The last keyboard state mask. */


public:

    /* CONSTRUCTORS */

    explicit InputManager(Game *game);


    /* METHODS */

    /**
     * @brief Handles the keyboard events.
     */
    void handleKeyboardEvents();

    /**
     * @brief Handles the key down event.
     * @param player The player object.
     * @param keyEvent The key event.
     */
    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent &keyEvent);

    /**
     * @brief Handles the key up event.
     * @param player The player object.
     * @param keyEvent The key event.
     */
    void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const;

    /**
     * @brief Sends the keyboard state to the network.
     */
    void sendKeyboardStateToNetwork();

    /**
     * @brief Sends the sync correction to the network.
     */
    void sendSyncCorrectionToNetwork() const;

private:

    /**
     * @brief Sends the keyboard state to the network.
     * @param lastKeyboardStateMask The last keyboard state mask.
     */
    void sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMask) const;

};

#endif //PLAY_TOGETHER_INPUTMANAGER_H
