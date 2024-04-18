#include "../../../include/Game/Platforms/WeightPlatform.h"

/**
 * @file WeightPlatform.cpp
 * @brief Implements the WeightPlatform class responsible for weight platform logic.
 */


/* CONSTRUCTORS */

WeightPlatform::WeightPlatform(float x, float y, float size, float stepDistance, const Texture& texture)
        : startY(y), x(x), size(size), stepDistance(stepDistance), texture(texture) {

    // Set the size
    textureOffsets = {texture.getOffsets().x * size, texture.getOffsets().y * size, texture.getOffsets().w * size, texture.getOffsets().h * size};
    w = static_cast<float>(texture.getSize().w) * size - (- textureOffsets.x + textureOffsets.w);
    h = static_cast<float>(texture.getSize().h) * size - (- textureOffsets.y + textureOffsets.h);
}


/* ACCESSORS */

float WeightPlatform::getX() const {
    return x;
}

float WeightPlatform::getY() const {
    return y;
}

float WeightPlatform::getW() const {
    return w;
}

float WeightPlatform::getH() const {
    return h;
}

float WeightPlatform::getMove() const {
    return move;
}

bool WeightPlatform::getIsMoving() const {
    return isMoving;
}

SDL_FRect WeightPlatform::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void WeightPlatform::setIsMoving(bool state) {
    isMoving = state;
}

void WeightPlatform::increaseWeight() {
    weight++;
}

void WeightPlatform::decreaseWeight() {
    weight--;
}


/* METHODS */

void WeightPlatform::applyMovement(double delta_time) {
    if (isMoving) {
        auto blend = static_cast<float>(1.0f - std::pow(0.5F, delta_time * lerpSmoothingFactor));
        float targetY = startY + weight * stepDistance;
        move = std::lerp(y, targetY, blend) - y;
        y += move;
        weight = 0; // Reset weight
    }
}

void WeightPlatform::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x + textureOffsets.x, y - camera.y + textureOffsets.y, w + textureOffsets.w, h + textureOffsets.h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void WeightPlatform::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}