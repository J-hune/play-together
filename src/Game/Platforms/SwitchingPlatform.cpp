#include "../../../include/Game/Platforms/SwitchingPlatform.h"

/**
 * @file SwitchingPlatform.cpp
 * @brief Implements the SwitchingPlatform class responsible for switching platform logic.
 */


/* CONSTRUCTORS */

SwitchingPlatform::SwitchingPlatform(float x, float y, float w, float h, const Texture& texture, Uint32 bpm, std::vector<Point> steps)
        : x(x), y(y), w(w), h(h), texture(texture), bpm(bpm), steps(std::move(steps)) {}


/* ACCESSORS */

float SwitchingPlatform::getX() const {
    return x;
}

float SwitchingPlatform::getY() const {
    return y;
}

float SwitchingPlatform::getW() const {
    return w;
}

float SwitchingPlatform::getH() const {
    return h;
}

bool SwitchingPlatform::getIsMoving() const {
    return isMoving;
}

SDL_FRect SwitchingPlatform::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void SwitchingPlatform::setIsMoving(bool state) {
    isMoving = state;
}


/* METHODS */

void SwitchingPlatform::applyMovement([[maybe_unused]] double delta_time) {
    Uint32 currentTime = SDL_GetTicks(); // Get the current time

    // Check if a beat has passed
    if (currentTime - startTime > (60000 / bpm)) {

        startTime = currentTime; // Reset the time
        actualPoint = (actualPoint + 1) % (int)steps.size(); // Move on to the next point

        // Apply the movement
        x = steps[actualPoint].x;
        y = steps[actualPoint].y;
    }
}

void SwitchingPlatform::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void SwitchingPlatform::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}