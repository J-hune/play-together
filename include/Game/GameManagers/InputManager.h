#ifndef PLAY_TOGETHER_INPUTMANAGER_H
#define PLAY_TOGETHER_INPUTMANAGER_H

#include <SDL_events.h>
#include "../Player.h"
#include "../Game.h"

class InputManager {
public:

    /** CONSTRUCTOR **/
    explicit InputManager(Game *game);


    /** METHODS **/

    void handleKeyboardEvents();

    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent &keyEvent);

    void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const;

private:
    Game *gamePtr; /**< A pointer to the game object. */

    /**
     * @brief Sends the keyboard state to the network.
     * @param lastKeyboardStateMask The last keyboard state mask.
     */
    void sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMask) const;
};

#endif //PLAY_TOGETHER_INPUTMANAGER_H
