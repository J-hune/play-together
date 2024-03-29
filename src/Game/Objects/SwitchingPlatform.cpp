#include "../../../include/Game/Objects/SwitchingPlatform.h"

/**
 * @file SwitchingPlatform.cpp
 * @brief Implements the SwitchingPlatform class responsible for switching platform logic.
 */

/** CONSTRUCTORS **/

SwitchingPlatform::SwitchingPlatform(float x, float y, float w, float h, Uint32 bpm, std::vector<Point> steps)
        : x(x), y(y), w(w), h(h), bpm(bpm), steps(std::move(steps)) {
    startTime = SDL_GetTicks(); // Get the current time
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

bool SwitchingPlatform::getIsMoving() const {
    return isMoving;
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
        actualPoint = (actualPoint + 1) % (int)steps.size(); // Move on to the next point

        // Apply the movement
        x = steps[actualPoint].x;
        y = steps[actualPoint].y;
    }
}