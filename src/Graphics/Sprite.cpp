#include "../../include/Graphics/Sprite.h"

/**
 * @file Sprite.cpp
 * @brief Implements the Sprite class responsible for handling sprite logic.
 */

/** CONSTRUCTORS **/

Sprite::Sprite(Animation animation, SDL_Texture &texture, int width, int height) :
        animation(animation), texturePtr(&texture), srcRect({0, 0, width, height}) {}


/** ACCESSORS **/

Animation Sprite::getAnimation() const {
    return animation;
}

SDL_Texture* Sprite::getTexture() const {
    return texturePtr;
}

SDL_Rect Sprite::getSrcRect() const {
    return srcRect;
}

SDL_RendererFlip Sprite::getFlip() const {
    return static_cast<SDL_RendererFlip>(flipVertical + flipHorizontal);
}


/** MODIFIERS **/

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

void Sprite::setTexture(SDL_Texture &newTexture) {
    texturePtr = &newTexture;
}

void Sprite::setFlipHorizontal(SDL_RendererFlip flip) {
    flipHorizontal = flip;
}

void Sprite::setFlipVertical(SDL_RendererFlip flip) {
    flipVertical = flip;
}

void Sprite::toggleFlipVertical() {
    flipVertical = (flipVertical == SDL_FLIP_VERTICAL) ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL;
}


/** METHODS **/

void Sprite::updateAnimation() {
    // If the animation was unique and is ended, switch to next animation
    if (animation.unique && nbFrameDisplayed == animation.frames) {
        uniqueAnimationIsDisplayed = false;
        setAnimation(nextAnimation);
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
}
