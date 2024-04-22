#include "../../../include/Game/Platforms/TreadmillLever.h"

/**
 * @file TreadmillLever.cpp
 * @brief Implements the TreadmillLever class responsible for treadmill lever logic.
 */


/* CONSTRUCTORS */

TreadmillLever::TreadmillLever(float x, float y, float size, bool is_activated, int type, const Texture& texture, const std::vector<Treadmill*> &treadmills)
        : Lever(x, y, size, is_activated, texture), type(type), treadmills(treadmills) {};


/* METHODS */

void TreadmillLever::applyEffect() const {
    bool is_activated = getIsActivated();

    for (auto treadmill : treadmills) {
        if (type == 1) {
            treadmill->setIsMoving(is_activated);
        }
        else if (type == 2) {
            treadmill->setDirection(is_activated ? 1 : -1);
        }
    }
}