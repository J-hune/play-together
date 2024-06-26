#include "../../../include/Game/Items/SpeedPowerUp.h"

/**
 * @file SpeedPowerUp.cpp
 * @brief Implements the SpeedPowerUp class responsible for SpeedPowerUp object.
 */


/* CONSTRUCTORS */

SpeedPowerUp::SpeedPowerUp(float x, float y, float w, float h, bool fast) :
            Item(x, y, w, h, "powerUp.wav"), fast(fast) {}


/* METHODS */

void SpeedPowerUp::applyEffect(Player &player) {
    Item::applyEffect(player);

    // The item is a fast power-up
    if (fast) {
        player.setSpeed(player.getSpeed() * 1.5F);
    }
    // The item is a slow power-up
    else {
        player.setSpeed(player.getSpeed() * 0.5F);
    }
}

void  SpeedPowerUp::inverseEffect(Player &player) {
    fast = !fast;
    applyEffect(player);
}

void SpeedPowerUp::render(SDL_Renderer *renderer, Point camera) {
    // Temporary render until sprite is implemented
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    Item::renderDebug(renderer, camera);
}