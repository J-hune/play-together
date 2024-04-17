#include "../../../include/Game/Platforms/MovingPlatform1D.h"

/**
 * @file MovingPlatform1D.cpp
 * @brief Implements the MovingPlatform1D class responsible for moving platforms logic.
 */


/* CONSTRUCTORS */

MovingPlatform1D::MovingPlatform1D(float x, float y, float w, float h, const Texture& texture, float speed, float min, float max, bool start, bool axis)
        : x(x), y(y), w(w), h(h), texture(texture), speed(speed), min(min), max(max), start(start), axis(axis) {

    // If the platform moves on x-axis
    if (!axis) {
        this->x = start ? max - w : min; // If the platform starts left or right
    }
    // If the platform moves on y-axis
    else {
        this->y = start ? max - h : min; // If the platform starts up or down
    }

    direction = start ? -1 : 1; // If the platform starts left/up, the direction is right/down, and vice-versa
}


/* ACCESSORS */

float MovingPlatform1D::getX() const {
    return x;
}

float MovingPlatform1D::getY() const {
    return y;
}

float MovingPlatform1D::getW() const {
    return w;
}

float MovingPlatform1D::getH() const {
    return h;
}

float MovingPlatform1D::getMove() const {
    return move;
}

bool MovingPlatform1D::getAxis() const {
    return axis;
}

bool MovingPlatform1D::getIsMoving() const {
    return isMoving;
}

SDL_FRect MovingPlatform1D::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void MovingPlatform1D::setIsMoving(bool state) {
    isMoving = state;
}


/* METHODS */

void MovingPlatform1D::applyXaxisMovement(double delta_time) {
    move = 150; // Add basic movement
    move *= static_cast<float>(delta_time); // Apply movement per second
    move *= speed; // Apply speed
    move *= direction; // Apply direction

    x += move; // Apply movement

    // If the platform has reached its minimum
    if (x < min) {
        x = min;
        direction = 1; // Change direction to left
    }
    // If the platform has reached its maximum
    else if (x + w > max) {
        x = max - w;
        direction = -1; // Change direction to right
    }
}

void MovingPlatform1D::applyYaxisMovement(double delta_time) {
    move = 150; // Add basic movement

    move *= static_cast<float>(delta_time); // Apply movement per second
    move *= speed; // Apply speed
    move *= direction; // Apply direction

    y += move; // Apply movement

    // If the platform has reached its minimum, change direction to left
    if (y < min) {
        y = min;
        direction = 1;
    }
    // If the platform has reached its maximum, change direction to right
    else if (y + h > max) {
        y = max - h;
        direction = -1;
    }
}

void MovingPlatform1D::applyMovement(double delta_time) {
    if (isMoving) {
        axis ? applyYaxisMovement(delta_time) : applyXaxisMovement(delta_time);
    } else move = 0;
}

void MovingPlatform1D::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void MovingPlatform1D::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}