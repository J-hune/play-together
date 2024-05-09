#include "../../../include/Game/Platforms/SwitchingPlatform.h"

/**
 * @file SwitchingPlatform.cpp
 * @brief Implements the SwitchingPlatform class responsible for switching platform logic.
 */


/* CONSTRUCTORS */

SwitchingPlatform::SwitchingPlatform(float x, float y, float size, Uint32 bpm, std::vector<Point> steps, const Texture& texture)
        : x(x), y(y), size(size), bpm(bpm), steps(std::move(steps)), texture(texture) {

    // Set the size
    textureOffsets = {texture.getOffsets().x * size, texture.getOffsets().y * size, texture.getOffsets().w * size, texture.getOffsets().h * size};
    w = static_cast<float>(texture.getSize().w) * size - (textureOffsets.x + textureOffsets.w);
    h = static_cast<float>(texture.getSize().h) * size - (textureOffsets.y + textureOffsets.h);
}


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

void SwitchingPlatform::setIsOnScreen(bool state) {
    isOnScreen = state;
}


/* METHODS */

void SwitchingPlatform::applyMovement([[maybe_unused]] double delta_time) {
    if (isMoving) {
        Uint32 currentTime = SDL_GetTicks(); // Get the current time

        // Check if a beat has passed
        if (currentTime - startTime > (60000 / bpm)) {

            startTime = currentTime; // Reset the time
            actualPoint = (actualPoint + 1) % (int) steps.size(); // Move on to the next point

            // Apply the movement
            x = steps[actualPoint].x;
            y = steps[actualPoint].y;
        }
    }
}

void SwitchingPlatform::render(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_Rect src_rect = texture.getSize();
        SDL_FRect platform_rect = {x - camera.x - textureOffsets.x, y - camera.y - textureOffsets.y,w + textureOffsets.w, h + textureOffsets.h};
        SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
    }
}

void SwitchingPlatform::renderDebug(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
        SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }
}