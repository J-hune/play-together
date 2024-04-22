#include "../../../include/Game/Traps/CrusherLever.h"

/**
 * @file CrusherLever.cpp
 * @brief Implements the CrusherLever class responsible for crusher lever logic.
 */


/* CONSTRUCTORS */

CrusherLever::CrusherLever(float x, float y, float size, bool is_activated, const Texture& texture, const std::vector<Crusher*> &crushers)
        : Lever(x, y, size, is_activated, texture), crushers(crushers) {};



/* METHODS */

void CrusherLever::applyEffect() const {
    bool is_activated = getIsActivated();
    for (auto crusher : crushers) {
        crusher->setIsMoving(is_activated);
    }
}