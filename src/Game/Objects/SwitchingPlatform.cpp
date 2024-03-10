#include "../../../include/Game/Objects/SwitchingPlatform.h"

/**
 * @file SwitchingPlatform.cpp
 * @brief Implements the SwitchingPlatform class responsible for switching platform logic.
 */

/** CONSTRUCTORS **/

SwitchingPlatform::SwitchingPlatform(float x, float y, float w, float h, Uint32 bpm, std::vector<Point> points)
        : x(x), y(y), w(w), h(h), bpm(bpm), points(std::move(points)) {
    startTime = SDL_GetTicks();
}


/** BASIC ACCESSORS **/

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


/** SPECIFIC ACCESSORS **/

SDL_FRect SwitchingPlatform::getBoundingBox() const {
    return {x, y, w, h};
}


/** MODIFIERS **/

void SwitchingPlatform::setIsMoving(bool state) {
    isMoving = state;
}


/** METHODS **/

void SwitchingPlatform::applyMovement() {
    Uint32 currentTime = SDL_GetTicks(); // Get the current time

    // Check if a beat has passed
    if (currentTime - startTime > (60000 / bpm)) {

        startTime = currentTime; // Reset the time
        actualPoint = (actualPoint + 1) % (int)points.size(); // Move on to the next point

        // Apply the movement
        x = points[actualPoint].x;
        y = points[actualPoint].y;
    }
}