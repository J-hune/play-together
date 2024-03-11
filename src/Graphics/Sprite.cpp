#include "../../include/Graphics/Sprite.h"

/**
 * @file Sprite.cpp
 * @brief Implements the Sprite class responsible for handling sprite logic.
 */

/** CONSTRUCTORS **/

Sprite::Sprite(Animation animation, SDL_Texture **texture, int width, int height) :
        animation(animation), texture(*texture), srcRect({0, 0, width, height}) {}


/** ACCESSORS **/

SDL_Texture* Sprite::getTexture() const {
    return texture;
}

SDL_Rect Sprite::getSrcRect() const {
    return srcRect;
}

SDL_RendererFlip Sprite::getFlip() const {
    return static_cast<SDL_RendererFlip>(flipVertical + flipHorizontal);
}


/** MODIFIERS **/

void Sprite::setAnimation(Animation newAnimation) {
    animation = newAnimation;
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
    srcRect.x = srcRect.w * (((int)SDL_GetTicks() / animation.speed) % animation.frames);
    srcRect.y = srcRect.h * animation.index;
}