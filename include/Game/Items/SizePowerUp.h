#ifndef PLAY_TOGETHER_SIZEPOWERUP_H
#define PLAY_TOGETHER_SIZEPOWERUP_H

#include "Item.h"
#include "../../Sounds/SoundEffect.h"

/**
 * @file SizePowerUp.h
 * @brief Defines the SizePowerUp class responsible for SizePowerUp object.
 */

/**
 * @class SizePowerUp
 * @brief Represents the SizePowerUp object.
 */
class SizePowerUp : public Item {
private:
    /* ATTRIBUTES */

    bool grow; /**< Flag indicating whether the power-up grow or shrink the player. */

public:
    /* CONSTRUCTORS */

    SizePowerUp(float x, float y, float w, float h, bool grow);
    ~SizePowerUp() override = default;


    /* METHODS */

    /**
    * @brief Apply the item's effect to a player.
    * @param player The player the effect will be applied to.
    * @param playerManager The manager responsible for handling player-related operations.
    */
    void applyEffect(Player &player, PlayerManager &playerManager) override;
};


#endif //PLAY_TOGETHER_SIZEPOWERUP_H
