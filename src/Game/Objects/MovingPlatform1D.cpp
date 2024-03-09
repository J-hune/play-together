#include "../../../include/Game/Objects/MovingPlatform1D.h"

/**
 * @file MovingPlatform1D.cpp
 * @brief Implements the MovingPlatform1D class responsible for moving platforms logic.
 */

/** CONSTRUCTORS **/

MovingPlatform1D::MovingPlatform1D(float x, float y, float w, float h, float speed, float min, float max, bool start, bool axis)
        : x(x), y(y), w(w), h(h), speed(speed), min(min), max(max), start(start), axis(axis) {

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


/** BASIC ACCESSORS **/

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

/** SPECIFIC ACCESSORS **/

SDL_FRect MovingPlatform1D::getBoundingBox() const {
    return {x, y, w, h};
}


/** METHODS **/

void MovingPlatform1D::applyXaxisMovement() {
    move = 1; // Apply movement

    float nearestPoint = std::abs(x - min) > std::abs((x + w) - max) ? max - w : min; // Calculate the nearest end to the platform
    float smoothing = (std::abs(x - nearestPoint) * PLATFORM_LERP_SMOOTHING_FACTOR); // Calculate smoothing

    move += smoothing > smoothingLimit ? smoothingLimit : smoothing; // Apply smoothing with limit
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

void MovingPlatform1D::applyYaxisMovement() {
    move = 1; // Apply movement

    float nearestPoint = std::abs(y - min) > std::abs((y + h) - max) ? max - h : min; // Calculate the nearest end to the platform
    float smoothing = (std::abs(y - nearestPoint) * PLATFORM_LERP_SMOOTHING_FACTOR); // Calculate smoothing

    move += smoothing > smoothingLimit ? smoothingLimit : smoothing; // Apply smoothing with limit
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

void MovingPlatform1D::applyMovingPlatformMovement() {
    axis ? applyYaxisMovement() : applyXaxisMovement();
}