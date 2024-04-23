#include "../../../include/Game/Platforms/MovingPlatform2D.h"


/**
 * @file MovingPlatform2D.cpp
 * @brief Implements the MovingPlatform2D class responsible for moving platforms logic.
 */


/* CONSTRUCTORS */

MovingPlatform2D::MovingPlatform2D(float x, float y, float size, float speed, Point left, Point right, bool start, const Texture& texture)
        : x(x), y(y), size(size), speed(speed), left(left), right(right),
         ratio(std::abs((left.x - right.x) / (left.y - right.y))), start(start), texture(texture) {

    // Set the size
    textureOffsets = {texture.getOffsets().x * size, texture.getOffsets().y * size, texture.getOffsets().w * size, texture.getOffsets().h * size};
    w = static_cast<float>(texture.getSize().w) * size - (textureOffsets.x + textureOffsets.w);
    h = static_cast<float>(texture.getSize().h) * size - (textureOffsets.y + textureOffsets.h);

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


/* ACCESSORS */

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

float MovingPlatform2D::getDirectionX() const {
    return directionX;
}

float MovingPlatform2D::getDirectionY() const {
    return directionY;
}

bool MovingPlatform2D::getIsMoving() const {
    return isMoving;
}

SDL_FRect MovingPlatform2D::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void MovingPlatform2D::setX(float value) {
    x = value;
}

void MovingPlatform2D::setY(float value) {
    y = value;
}

void MovingPlatform2D::setMoveX(float value) {
    moveX = value;
}

void MovingPlatform2D::setMoveY(float value) {
    moveY = value;
}

void MovingPlatform2D::setDirectionX(float value) {
    directionX = value;
}

void MovingPlatform2D::setDirectionY(float value) {
    directionY = value;
}

void MovingPlatform2D::setBuffer(PlatformBuffer value) {
    buffer = value;
}

void MovingPlatform2D::setIsMoving(bool state) {
    isMoving = state;
}

void MovingPlatform2D::setIsOnScreen(bool state) {
    isOnScreen = state;
}


/* METHODS */

void MovingPlatform2D::applyMovement(double delta_time) {
    if (isMoving && isOnScreen) {
        // Add basic movement
        moveX = 150;
        moveY = 150;

        // Calculate x-axis movement
        moveX *= static_cast<float>(delta_time); // Apply movement per second
        moveX *= speed; // Apply speed
        moveX *= directionX; // Apply direction

        // If the buffer is too big, move the platform and reset the buffer
        if (buffer.deltaX > 40 || buffer.deltaX < -40 || buffer.deltaY > 40 || buffer.deltaY < -40) {
            x = x + buffer.deltaX;
            y = y + buffer.deltaY;

            buffer.deltaX = 0;
            buffer.deltaY = 0;
        }

        else {
            // Calculate y-axis movement
            moveY *= static_cast<float>(delta_time); // Apply movement per second
            moveY *= speed; // Apply speed
            moveY *= directionY; // Apply direction

            // Apply ratio
            ratio > 1 ? moveY /= ratio : moveX *= ratio;


            // Add a part of the buffer to the platform's position
            float bufferFraction = static_cast<float>(delta_time) * 1000.0f / 50.0f;

            // Apply movement to the platform
            x += moveX + bufferFraction * buffer.deltaX;
            y += moveY + bufferFraction * buffer.deltaY;

            // Decrease the buffer
            buffer.deltaX -= bufferFraction * buffer.deltaX;
            buffer.deltaY -= bufferFraction * buffer.deltaY;
        }

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
    } else{
        moveX = 0;
        moveY = 0;
    }
}

void MovingPlatform2D::render(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_Rect src_rect = texture.getSize();
        SDL_FRect platform_rect = {x - camera.x - textureOffsets.x, y - camera.y - textureOffsets.y,w + textureOffsets.w, h + textureOffsets.h};
        SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
    }
}

void MovingPlatform2D::renderDebug(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
        SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }
}