#include "../../include/Graphics/Sprite.h"

/**
 * @file Sprite.cpp
 * @brief Implements the Sprite class responsible for handling sprite logic.
 */


/* CONSTRUCTORS */

Sprite::Sprite(SDL_Texture &texture, Animation animation, int width, int height) :
        Texture(texture), animation(animation), srcRect({0, 0, width, height}) {}


/* ACCESSORS */

Animation Sprite::getAnimation() const {
    return animation;
}

SDL_Rect Sprite::getSrcRect() const {
    return srcRect;
}


/* MODIFIERS */

void Sprite::setAnimation(const Animation& newAnimation) {
    // Change animation only if it's different
    if (animation != newAnimation) {
        // Change animation only if the current is not unique or if the unique animation is ended
        if (!animation.unique || (animation.unique && nbFrameDisplayed == animation.frames)) {

            // If the new animation is 'unique'
            if (newAnimation.unique) {
                nextAnimation = animation; // Store the current animation to display it after the unique animation ends
                uniqueAnimationIsDisplayed = true;
            }

            // Change animation
            animation = newAnimation;
            animationIndexX = 0;
            nbFrameDisplayed = 0;
            lastAnimationUpdate = SDL_GetTicks();

        }
        // Else, the current animation is unique, store the new animation to display it after the unique animation ends
        else {
            nextAnimation = newAnimation;
        }
    }
}

void Sprite::setAnimationIndexX(int index) {
    animationIndexX = index;
}


/* METHODS */

bool Sprite::updateAnimation() {
    bool check = false;

    // If the animation was unique and is ended, switch to the next animation
    if (animation.unique && nbFrameDisplayed == animation.frames) {
        uniqueAnimationIsDisplayed = false;
        setAnimation(nextAnimation);
        check = true;
    }

    // Update x position animation
    if (SDL_GetTicks() - lastAnimationUpdate > animation.speed) {
        lastAnimationUpdate = SDL_GetTicks() ;
        animationIndexX = (animationIndexX + 1) % animation.frames;
        nbFrameDisplayed++;
    }

    // Apply animation
    srcRect.x = srcRect.w * animationIndexX;
    srcRect.y = srcRect.h * animation.indexY;

    return check;
}
