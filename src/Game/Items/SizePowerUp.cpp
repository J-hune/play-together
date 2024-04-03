#include "../../../include/Game/Items/SizePowerUp.h"

/**
 * @file SizePowerUp.cpp
 * @brief Implements the SizePowerUp class responsible for SizePowerUp object.
 */

/* CONSTRUCTORS */

SizePowerUp::SizePowerUp(float x, float y, float w, float h, bool grow) : Item(x, y, w, h), grow(grow) {}


/* METHODS */

void SizePowerUp::applyEffect(Player &player) const {
    Item::applyEffect(player);

    // The item is a growth power-up
    if (grow) {
            player.setW(player.getW() * 2);
            player.setH(player.getH() * 2);
    }
    // The item is a shrink power-up
    else {
        player.setW(player.getW() / 2);
        player.setH(player.getH() / 2);
    }
}
