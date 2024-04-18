#include "../../../include/Game/GameManagers/InputManager.h"

/**
 * @file InputManager.cpp
 * @brief Implements the InputManager class responsible for handling the input events.
 */


/* CONSTRUCTOR */

InputManager::InputManager(Game *game) : gamePtr(game) {}


/* METHODS */

void InputManager::handleKeyboardEvents() {
    SDL_Event e;
    Player *playerPtr = gamePtr->getPlayerManager().findPlayerById(-1);

    // Main loop handling every event one by one
    while (SDL_PollEvent(&e) != 0) {
        // Handle SDL_QUIT event
        if (e.type == SDL_QUIT) {
            std::cout << "InputManager: Quit event detected\n" << std::endl;
            gamePtr->exitGame();
            gamePtr->stop();
        }

        // Handle SDL_KEYUP event for the escape key
        if (e.type == SDL_KEYUP && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gamePtr->togglePause();
        }

        // If the game is paused, handle menu events and skip the rest
        if (gamePtr->getGameState() == GameState::PAUSED) {
            Mediator::handleEventMenu(e);
            continue;
        }

        // Handle key events
        if (e.type == SDL_KEYUP) {
            handleKeyUpEvent(playerPtr, e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(playerPtr, e.key);
        }

            // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            float cameraX = gamePtr->getCamera()->getX();
            float cameraY = gamePtr->getCamera()->getY();
            printf("InputManager: Mouse clicked at (%f, %f)\n", (float)e.button.x + cameraX, (float)e.button.y + cameraY);
        }
    }

    // If the player is alive, send the keyboard state to the network after handling all events
    if (playerPtr != nullptr) sendKeyboardStateToNetwork();
}

void InputManager::handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const {
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            if (player == nullptr) break;
            player->setWantToJump(false); // Disable jumping
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;

            // Reset horizontal movement if moving left
            if (player->getMoveX() < 0) player->setMoveX(0);
            player->setWantToMoveLeft(false); // Disable left movement
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;

            // Reset horizontal movement if moving right
            if (player->getMoveX() > 0) {
                player->setMoveX(0);
            }
            player->setWantToMoveRight(false); // Disable right movement
            break;
        case SDL_SCANCODE_LSHIFT:
            if (player == nullptr) break;
            player->setSprint(false);
            break;
        default:
            break;
    }
}

void InputManager::handleKeyDownEvent(Player *player, const SDL_KeyboardEvent &keyEvent) {
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            // If the player is dead, ignore
            if (player == nullptr) break;

            // If the coyote time is passed and the player is not already in a jump
            if (!player->getIsJumping()) {
                player->setWantToJump(true);
            }
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;
            player->setWantToMoveLeft(true);
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;
            player->setWantToMoveRight(true);
            break;
        case SDL_SCANCODE_G:
            gamePtr->switchMavity(); // Switch mavity for all players
            break;
        case SDL_SCANCODE_M:
            if (gamePtr->getLevel()->getMapID() == 1) gamePtr->setLevel("assurance");
            else gamePtr->setLevel("diversity");
            break;
        case SDL_SCANCODE_LSHIFT:
            if (player == nullptr) break;
            player->setSprint(true);
            break;
        default:
            break;
    }
}


void InputManager::sendKeyboardStateToNetwork() {

    // Check if the game is in development mode
#ifdef DEVELOPMENT_MODE
    bool isDevelopmentMode = true;
#else
    bool isDevelopmentMode = false;
#endif

    // Get the current keyboard state mask
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    uint16_t currentKeyboardStateMask = Mediator::encodeKeyboardStateMask(keyboardState);

    // If the keyboard state has changed since the last message was sent
    if (!isDevelopmentMode || currentKeyboardStateMask != lastKeyboardStateMask) {
        // Send the new keyboard state mask to the server
        Mediator::sendPlayerUpdate(currentKeyboardStateMask);

        // Update the last keyboard state mask and the last send-time
        lastKeyboardStateMask = currentKeyboardStateMask;
    }
}

void InputManager::sendSyncCorrectionToNetwork() const {
    using json = nlohmann::json;
    json message;

    message["messageType"] = "syncCorrection";
    Mediator::sendSyncCorrection(message);
}