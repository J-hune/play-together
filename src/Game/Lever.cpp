#include "../../include/Game/Lever.h"

/**
 * @file Lever.cpp
 * @brief Implements the Lever class responsible for lever logic.
 */


/* CONSTRUCTORS */

Lever::Lever(float x, float y, float size, bool is_activated, const Texture& texture)
        : x(x), y(y), size(size), isActivated(is_activated), texture(texture) {

    // Set the size
    textureOffsets = {texture.getOffsets().x * size, texture.getOffsets().y * size, texture.getOffsets().w * size, texture.getOffsets().h * size};
    w = static_cast<float>(texture.getSize().w) * size - (textureOffsets.x + textureOffsets.w);
    h = static_cast<float>(texture.getSize().h) * size - (textureOffsets.y + textureOffsets.h);
    this->y -= h;

    if (!isActivated) this->texture.setFlipHorizontal(SDL_FLIP_HORIZONTAL);

}


/* ACCESSORS */

float Lever::getX() const {
    return x;
}

float Lever::getY() const {
    return y;
}

float Lever::getW() const {
    return w;
}

float Lever::getH() const {
    return h;
}

bool Lever::getIsActivated() const {
    return isActivated;
}

SDL_FRect Lever::getBoundingBox() const {
    return {x, y, w, h};
}


/* MUTATORS */

void Lever::setIsActivated(bool is_activated) {
    isActivated = is_activated;
    if (isActivated) {
        texture.setFlipHorizontal(SDL_FLIP_NONE);
    }
    else {
        texture.setFlipHorizontal(SDL_FLIP_HORIZONTAL);
    }
}

void Lever::toggleIsActivated() {
    setIsActivated(!isActivated);
    activationSound.play(0, -1);
    applyEffect();
}


/* METHODS */

bool Lever::operator==(const Lever &item) const {
    return x == item.getX()
           && y == item.getY()
           && w == item.getW()
           && h == item.getH();
}

void Lever::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x - textureOffsets.x, y - camera.y - textureOffsets.y, w + textureOffsets.w, h + textureOffsets.h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void Lever::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 102, 51, 0, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}