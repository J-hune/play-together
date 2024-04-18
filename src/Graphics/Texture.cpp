#include "../../include/Graphics/Texture.h"

/**
 * @file Texture.cpp
 * @brief Implements the Texture class responsible for handling texture logic.
 */


/* CONSTRUCTORS */

Texture::Texture(SDL_Texture &texture, SDL_FRect offsets) : texturePtr(&texture), offsets(offsets) {
    SDL_QueryTexture(&texture, nullptr, nullptr, &sizeRect.w, &sizeRect.h);
}


/* ACCESSORS */

SDL_Texture* Texture::getTexture() const {
    return texturePtr;
}

SDL_Rect Texture::getSize() const {
    return sizeRect;
}

SDL_FRect Texture::getOffsets() const {
    return offsets;
}

SDL_RendererFlip Texture::getFlip() const {
    return static_cast<SDL_RendererFlip>(flipVertical + flipHorizontal);
}


/* MODIFIERS */

void Texture::setTexture(SDL_Texture &newTexture) {
    texturePtr = &newTexture;
}

void Texture::setFlipHorizontal(SDL_RendererFlip flip) {
    flipHorizontal = flip;
}

void Texture::setFlipVertical(SDL_RendererFlip flip) {
    flipVertical = flip;
}

void Texture::toggleFlipVertical() {
    flipVertical = (flipVertical == SDL_FLIP_VERTICAL) ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL;
}
