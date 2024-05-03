#include "../../../include/Game/Platforms/Treadmill.h"

/**
 * @file Treadmill.cpp
 * @brief Implements the Treadmill class responsible for treadmill logic.
 */


// Static member initialization
SDL_Texture *Treadmill::spriteTexturePtr = nullptr;
int Treadmill::SPRITE_WIDTH = 0;
int Treadmill::SPRITE_HEIGHT = 0;


/* CONSTRUCTORS */

Treadmill::Treadmill(float x, float y, float size, float speed, float direction, Uint32 spriteSpeed)
                    : x(x), y(y), size(size), speed(speed), direction(direction), spriteSpeed(spriteSpeed) {
    w = static_cast<float>(SPRITE_WIDTH) * size;
    h = static_cast<float>(SPRITE_HEIGHT) * size;
    sprite = Sprite(*spriteTexturePtr, direction > 0 ? right : left, SPRITE_WIDTH, SPRITE_HEIGHT);
}


/* ACCESSORS */

float Treadmill::getX() const {
    return x;
}

float Treadmill::getY() const {
    return y;
}

float Treadmill::getW() const {
    return w;
}

float Treadmill::getH() const {
    return h;
}

float Treadmill::getSpeed() const {
    return speed;
}

float Treadmill::getDirection() const {
    return direction;
}

float Treadmill::getMove() const {
    return move;
}

bool Treadmill::getIsMoving() const {
    return isMoving;
}

SDL_FRect Treadmill::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void Treadmill::setDirection(float new_direction) {
    direction = new_direction;
    if (direction > 0) {
        sprite.setAnimation(right);
    } else {
        sprite.setAnimation(left);
    }
}

void Treadmill::setIsMoving(bool state) {
    isMoving = state;
}

void Treadmill::setIsOnScreen(bool state) {
    isOnScreen = state;
}

void Treadmill::setTexture(SDL_Texture *texturePtr) {
    spriteTexturePtr = texturePtr;

    int w;
    int h;
    SDL_QueryTexture(spriteTexturePtr, nullptr, nullptr, &w, &h);
    SPRITE_WIDTH = w / FRAME_NUMBER;
    SPRITE_HEIGHT = h / 2;
}

/* METHODS */

void Treadmill::calculateMovement(double delta_time) {
    if (isMoving && isOnScreen) {
        move = 64;
        move *= speed;
        move *= direction;
        move *= static_cast<float>(delta_time);
    } else {
        move = 0;
    }

}

void Treadmill::render(SDL_Renderer *renderer, Point camera) {
    if (isOnScreen) {
        if (isMoving) sprite.updateAnimation();
        SDL_Rect srcRect = sprite.getSrcRect();
        SDL_FRect treadmill_rect = {x - camera.x, y - camera.y, w, h};
        SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &treadmill_rect, 0.0, nullptr, sprite.getFlip());
    }
}

void Treadmill::renderDebug(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_SetRenderDrawColor(renderer, 154, 153, 150, 255);
        SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }
}