#include "../../../include/Game/Platforms/PlatformLever.h"

/**
 * @file PlatformLever.cpp
 * @brief Implements the PlatformLever class responsible for platform lever logic.
 */


/* CONSTRUCTORS */

PlatformLever::PlatformLever(float x, float y, float size, bool is_activated, const Texture& texture, const std::vector<MovingPlatform1D*> &platforms1D, const std::vector<MovingPlatform2D*> &platforms2D)
        : Lever(x, y, size, is_activated, texture), movingPlatform1D(platforms1D), movingPlatform2D(platforms2D) {};


/* METHODS */

void PlatformLever::applyEffect() const {
    bool is_activated = getIsActivated();

    // 1D moving platforms
    for (auto platform : movingPlatform1D) {
        platform->setIsMoving(is_activated);
    }

    // 2D moving platforms
    for (auto platform : movingPlatform2D) {
        platform->setIsMoving(is_activated);
    }
}
