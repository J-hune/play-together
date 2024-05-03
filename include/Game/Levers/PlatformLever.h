#ifndef PLAY_TOGETHER_PLATFORMLEVER_H
#define PLAY_TOGETHER_PLATFORMLEVER_H

#include "Lever.h"
#include "../Platforms/MovingPlatform1D.h"
#include "../Platforms/MovingPlatform2D.h"


/**
 * @file PlatformLever.h
 * @brief Defines the PlatformLever class representing a platform lever.
 */
class PlatformLever : public Lever {
private:
    /* ATTRIBUTES */

    std::vector<MovingPlatform1D*> movingPlatform1D; /**< The 1D moving platforms affected by the lever. */
    std::vector<MovingPlatform2D*> movingPlatform2D; /**< The 2D moving platforms affected by the lever. */


public:
    /* CONSTRUCTORS */

    PlatformLever(float x, float y, float size, bool is_activated, const Texture& texture, const std::vector<MovingPlatform1D*> &platforms1D, const std::vector<MovingPlatform2D*> &platforms2D);


private:

    /* METHODS */

    /**
     * @brief Apply the effect of the lever on the platforms.
     */
    void applyEffect() const override;
};


#endif //PLAY_TOGETHER_PLATFORMLEVER_H
