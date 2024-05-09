#include "../../../include/Game/Items/SizePowerUp.h"

/**
 * @file SizePowerUp.cpp
 * @brief Implements the SizePowerUp class responsible for SizePowerUp object.
 */


/* CONSTRUCTORS */

SizePowerUp::SizePowerUp(float x, float y, float w, float h, bool grow) :
            Item(x, y, w, h, "powerUp.wav"), grow(grow) {}


/* METHODS */

void SizePowerUp::inverseEffect(Player &player) {
    grow = !grow;
    applyEffect(player);
}

void SizePowerUp::applyEffect(Player &player) {
    //Item::applyEffect(player);

    // The item is a growth power-up
    if (grow) {
        player.setY(player.getY() - (player.getH() * 2 - player.getH())); // Move player up to avoid collision with the ground
        player.setSize(player.getSize() * 2);
    }
    // The item is a shrink power-up
    else {
        player.setSize(player.getSize() / 2);
    }
}

void SizePowerUp::render(SDL_Renderer *renderer, Point camera) {
    // Temporary render until sprite is implemented
    SDL_SetRenderDrawColor(renderer, 0, 255, 180, 255);
    Item::renderDebug(renderer, camera);
}
