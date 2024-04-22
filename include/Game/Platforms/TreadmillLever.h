#ifndef PLAY_TOGETHER_TREADMILLLEVER_H
#define PLAY_TOGETHER_TREADMILLLEVER_H

#include "../Lever.h"
#include "Treadmill.h"

/**
 * @file TreadmillLever.h
 * @brief Defines the TreadmillLever class representing a treadmill lever.
 */
class TreadmillLever : public Lever {
private:
    /* ATTRIBUTES */

    int type; /**< The type of the treadmill lever, 1: activation, 2: direction. */
    std::vector<Treadmill*> treadmills; /**< The treadmills affected by the lever. */



public:
    /* CONSTRUCTORS */

    TreadmillLever(float x, float y, float size, bool is_activated, int type, const Texture& texture, const std::vector<Treadmill*> &treadmills);


private:

    /* METHODS */

    /**
     * @brief Apply the effect of the lever on the treadmills.
     */
    void applyEffect() const override;
};


#endif //PLAY_TOGETHER_TREADMILLLEVER_H
