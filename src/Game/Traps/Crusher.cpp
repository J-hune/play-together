#include "../../../include/Game/Traps/Crusher.h"

/* CONSTRUCTORS */

Crusher::Crusher(float x, float y, float size, float min, float max, Uint32 moveUpTime, Uint32 waitUpTime, Uint32 waitDownTime, const Texture& texture)
        : x(x), y(y), size(size), min(min), max(max), pixelToMove(std::abs(max - min) / static_cast<float>(moveUpTime)),
        moveUpTime(moveUpTime), waitUpTime(waitUpTime), waitDownTime(waitDownTime), texture(texture) {

    // Set the size
    textureOffsets = {texture.getOffsets().x * size, texture.getOffsets().y * size, texture.getOffsets().w * size, texture.getOffsets().h * size};
    w = static_cast<float>(texture.getSize().w) * size - (textureOffsets.x + textureOffsets.w);
    h = static_cast<float>(texture.getSize().h) * size - (textureOffsets.y + textureOffsets.h);
}


/* ACCESSORS */

float Crusher::getX() const {
    return x;
}

float Crusher::getY() const {
    return y;
}

float Crusher::getW() const {
    return w;
}

float Crusher::getH() const {
    return h;
}

bool Crusher::getIsCrushing() const {
    return isCrushing;
}

bool Crusher::getIsMoving() const {
    return isMoving;
}

bool Crusher::getIsOnScreen() const {
    return isOnScreen;
}

SDL_FRect Crusher::getBoundingBox() const {
    return {x, y, w, h};
}

SDL_FRect Crusher::getCrushingZoneBoundingBox() const {
    return  {x + 1, y + h, w - 2, 1};
}


/* MODIFIERS */

void Crusher::setIsMoving(bool state) {
    isMoving = state;
    if (!state) {
        isCrushing = false;
    }
}

void Crusher::setIsOnScreen(bool state) {
    isOnScreen = state;
}


/* METHODS */

void Crusher::applyUpMovement(double delta_time) {
    auto blend = static_cast<float>(1 - std::pow(0.5F, delta_time * 20));
    float smooth_movement = (max - y + 0.1F) * blend;
    float normal_movement = pixelToMove * static_cast<float>(delta_time);
    y -= std::min(smooth_movement, normal_movement);

    // The movement is finished
    if (y <= min) {
        y = min;
        direction = 1;
        timer = static_cast<int>(waitUpTime);
        lastUpdate = SDL_GetTicks();
    }
}

bool Crusher::applyDownMovement(double delta_time) {
    auto blend = static_cast<float>(1 - std::pow(0.5F, delta_time * 10));
    y += (y - min + 0.1F) * blend;

    // The crusher can kill only if he reached the half of his down movement
    if (y - min > (max - min) / 2) isCrushing = true;

    // The movement is finished
    if (y >= max) {
        y = max;
        direction = -1;
        isCrushing = false;
        crushingSound.play(0, -1);
        timer = static_cast<int>(waitDownTime);
        lastUpdate = SDL_GetTicks();
        return true;
    }

    return false;
}

bool Crusher::applyMovement(double delta_time) {
    bool check = false;

    if (isMoving && isOnScreen) {
        // The crusher is in a waiting state
        if (timer > 0) {
            timer -= static_cast<int>(SDL_GetTicks() - lastUpdate);
            lastUpdate = SDL_GetTicks();
        }
        // The crusher is moving
        else {
            if (direction == 1) {
                check = applyDownMovement(delta_time);
            } else {
                applyUpMovement(delta_time);
            }
        }
    }

    return check;
}

void Crusher::render(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_Rect src_rect = texture.getSize();
        SDL_FRect platform_rect = {x - camera.x - textureOffsets.x, y - camera.y - textureOffsets.y,w + textureOffsets.w, h + textureOffsets.h};
        SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
    }
}

void Crusher::renderDebug(SDL_Renderer *renderer, Point camera) const {
    if (isOnScreen) {
        SDL_SetRenderDrawColor(renderer, 249, 190, 152, 255);
        SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }
}