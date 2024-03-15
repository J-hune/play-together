#include "../../../include/Game/Objects/MovingPlatform2D.h"


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


/** SPECIFIC ACCESSORS **/

SDL_FRect MovingPlatform2D::getBoundingBox() const {
    return {x, y, w, h};
}


/** MODIFIERS **/

void MovingPlatform2D::setIsMoving(bool state) {
    isMoving = state;
}


/** METHODS **/

void MovingPlatform2D::applyMovement() {
    // Apply movement
    moveX = 1;
    moveY = 1;

    // Calculate distances between the platform and the two points
    float distancePoint1 = ((x - left.x) * (x - left.x) + (y - left.y) * (y - left.y));
    float distancePoint2 = ((x - right.x) * (x - right.x) + (y - right.y) * (y - right.y));

    // Calculate the nearest point to the platform
    float smoothingX = distancePoint1 > distancePoint2 ? right.x - w : left.x;
    float smoothingY = distancePoint1 > distancePoint2 ? right.y - h : left.y;

    // Calculate smoothing
    smoothingX = (std::abs(x - smoothingX) * 0.5F);
    smoothingY = (std::abs(y - smoothingY) * 0.5F);

    // Calculate x-axis movement
    moveX += smoothingX > smoothingLimit ? smoothingLimit : smoothingX; // Apply smoothing with limit
    moveX *= speed; // Apply speed
    moveX *= directionX; // Apply direction

    // Calculate y-axis movement
    moveY += smoothingY > smoothingLimit ? smoothingLimit : smoothingY; // Apply smoothing with limit
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