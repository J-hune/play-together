#ifndef PLAY_TOGETHER_ANIMATION_H
#define PLAY_TOGETHER_ANIMATION_H

struct Animation {
    int indexY; /**< The y position on the sprite sheet. */
    int frames; /**< The number of frame to display. */
    Uint32 speed; /**< The speed between each frame. */
    bool unique; /**< Flag indicating if the animation should be display only once. */
};

#endif //PLAY_TOGETHER_ANIMATION_H
