#ifndef PLAY_TOGETHER_SPEEDPOWERUP_H
#define PLAY_TOGETHER_SPEEDPOWERUP_H

#include "Item.h"

/**
 * @file SpeedPowerUp.h
 * @brief Defines the SpeedPowerUp class responsible for SpeedPowerUp object.
 */

/**
 * @class SpeedPowerUp
 * @brief Represents the SpeedPowerUp object.
 */
class SpeedPowerUp : public Item {
private:
    /* ATTRIBUTES */

    bool fast; /**< Flag indicating whether the power-up makes the player faster or slower. */

public:
    /* CONSTRUCTORS */

    SpeedPowerUp(float x, float y, float w, float h, bool fast);
    ~SpeedPowerUp() override = default;


    /* METHODS */

    /**
     * @brief Apply the item's effect to a player.
     * @param player The player the effect will be applied to.
     */
    void applyEffect(Player &player) override;
};


#endif //PLAY_TOGETHER_SPEEDPOWERUP_H
