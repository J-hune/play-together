#include "../../../include/Game/Platforms/WeightPlatform.h"

/**
 * @file WeightPlatform.cpp
 * @brief Implements the WeightPlatform class responsible for weight platform logic.
 */


/* CONSTRUCTORS */

WeightPlatform::WeightPlatform(float x, float y, float w, float h, float stepDistance, const Texture& texture)
        : startY(y), x(x), w(w), h(h), stepDistance(stepDistance), texture(texture) {}


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
    auto blend = static_cast<float>(1.0f - std::pow(0.5F, delta_time * lerpSmoothingFactor));
    float targetY = startY + weight * stepDistance;
    y = lerp(y, targetY, blend);
    weight = 0; // Reset weight
}

void WeightPlatform::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void WeightPlatform::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}