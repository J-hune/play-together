#include "../../../include/Game/Platforms/MovingPlatform2D.h"


/**
 * @file MovingPlatform2D.cpp
 * @brief Implements the MovingPlatform2D class responsible for moving platforms logic.
 */

/** CONSTRUCTORS **/

MovingPlatform2D::MovingPlatform2D(float x, float y, float w, float h, float speed, Point left, Point right, bool start)
        : x(x), y(y), w(w), h(h), speed(speed), left(left), right(right),
         ratio(std::abs((left.x - right.x) / (left.y - right.y))), start(start) {

    // If the platform starts to the min point
    if (!start) {
        this->x = left.x;
        this->y = left.y;
        directionX = 1;
        directionY =  left.y - right.y > 0 ? -1 : 1;
    }
    // If the platform starts to the max point
    else {
        this->x = right.x - w;
        this->y = right.y - h;
        directionX = -1;
        directionY =  left.y - right.y > 0 ? 1 : -1;
    }
}


/** BASIC ACCESSORS **/

float MovingPlatform2D::getX() const {
    return x;
}

float MovingPlatform2D::getY() const {
    return y;
}

float MovingPlatform2D::getW() const {
    return w;
}

float MovingPlatform2D::getH() const {
    return h;
}

float MovingPlatform2D::getMoveX() const {
    return moveX;
}

float MovingPlatform2D::getMoveY() const {
    return moveY;
}

bool MovingPlatform2D::getIsMoving() const {
    return isMoving;
}

/** SPECIFIC ACCESSORS **/

SDL_FRect MovingPlatform2D::getBoundingBox() const {
    return {x, y, w, h};
}


/** MODIFIERS **/

void MovingPlatform2D::setIsMoving(bool state) {
    isMoving = state;
}


/** METHODS **/

void MovingPlatform2D::applyMovement(double deltaTime) {
    // Add basic movement
    moveX = 150;
    moveY = 150;

    // Calculate x-axis movement
    moveX *= deltaTime; // Apply movement per second
    moveX *= speed; // Apply speed
    moveX *= directionX; // Apply direction

    // Calculate y-axis movement
    moveY *= deltaTime; // Apply movement per second
    moveY *= speed; // Apply speed
    moveY *= directionY; // Apply direction

    // Apply ratio
    ratio > 1 ? moveY /= ratio : moveX *= ratio;

    // Apply movement to the platform
    x += moveX;
    y += moveY;

    // If the platform has reached its minimum point
    if (x < left.x) {
        x = left.x;
        y = left.y;
        directionX = 1; // Change direction to left
        directionY = left.y - right.y > 0 ? -1 : 1;
    }
    // If the platform has reached its maximum point

    else if (x + w > right.x) {
        x = right.x - w;
        directionX = -1; // Change direction to right
        directionY = left.y - right.y > 0 ? 1 : -1;
    }
}