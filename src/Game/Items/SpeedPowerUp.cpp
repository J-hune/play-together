#include "../../../include/Game/Items/SpeedPowerUp.h"

/**
 * @file SpeedPowerUp.cpp
 * @brief Implements the SpeedPowerUp class responsible for SpeedPowerUp object.
 */

// Static member initialization


/* CONSTRUCTORS */

SpeedPowerUp::SpeedPowerUp(float x, float y, float w, float h, bool fast) :
            Item(x, y, w, h, "powerUp.wav"), fast(fast) {}


/* METHODS */

void SpeedPowerUp::applyEffect(Player &player, PlayerManager &playerManager) {
    Item::applyEffect(player, playerManager);

    // The item is a fast power-up
    if (fast) {
        player.setSpeed(player.getSpeed() * 1.5F);
    }
    // The item is a slow power-up
    else {
        player.setSpeed(player.getSpeed() * 0.5F);
    }
}
