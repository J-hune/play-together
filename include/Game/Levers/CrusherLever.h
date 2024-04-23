#ifndef PLAY_TOGETHER_CRUSHERLEVER_H
#define PLAY_TOGETHER_CRUSHERLEVER_H

#include <vector>
#include "Lever.h"
#include "../Traps/Crusher.h"

/**
 * @file CrusherLever.h
 * @brief Defines the CrusherLever class representing a crusher lever.
 */

/**
 * @class CrusherLever
 * @brief Represents a crusher lever in a 2D game.
 */
class CrusherLever : public Lever {
private:
    /* ATTRIBUTES */

    std::vector<Crusher*> crushers; /**< The crushers affected by the lever. */


public:
    /* CONSTRUCTORS */

    /**
     * @brief Constructs a CrusherLever object.
     * @param x The x-coordinate of the lever.
     * @param y The y-coordinate of the lever.
     * @param size The size of the lever.
     * @param is_activated The activation state of the lever.
     * @param texture The texture of the lever.
     * @param crushers The crushers affected by the lever.
     */
    CrusherLever(float x, float y, float size, bool is_activated, const Texture& texture, const std::vector<Crusher*> &crushers);


private:

    /* METHODS */

    /**
     * @brief Apply the effect of the lever on the crushers.
     */
    void applyEffect() const override;

};


#endif //PLAY_TOGETHER_CRUSHERLEVER_H
