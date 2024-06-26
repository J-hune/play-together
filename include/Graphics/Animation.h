#ifndef PLAY_TOGETHER_ANIMATION_H
#define PLAY_TOGETHER_ANIMATION_H

#include <SDL_stdinc.h>

struct Animation {
    int indexY; /**< The y position on the sprite sheet. */
    int frames; /**< The number of frame to display. */
    Uint32 speed; /**< The speed between each frame. */
    bool unique; /**< Flag indicating if the animation should be display only once. */

    /**
     * @brief Overloaded equality operator.
     * @param item The item object to compare with.
     * @return True if the animations are equal, false otherwise.
     */
    bool operator==(const Animation& other) const {
        return (indexY == other.indexY && frames == other.frames && speed == other.speed);
    }
};

#endif //PLAY_TOGETHER_ANIMATION_H
