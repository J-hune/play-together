#include "../../../include/Game/Traps/Crusher.h"

/* CONSTRUCTORS */

Crusher::Crusher(float x, float y, float w, float h, float min, float max, Uint32 moveUpTime, Uint32 waitUpTime, Uint32 waitDownTime, const Texture& texture)
        : x(x), y(y), w(w), h(h), min(min), max(max), pixelToMove(std::abs(max - min) / static_cast<float>(moveUpTime)),
        moveUpTime(moveUpTime), waitUpTime(waitUpTime), waitDownTime(waitDownTime), texture(texture) {}


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

SDL_FRect Crusher::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

void Crusher::setIsMoving(bool state) {
    isMoving = state;
    isCrushing = state;
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

void Crusher::applyDownMovement(double delta_time) {
    isCrushing = true;

    auto blend = static_cast<float>(1 - std::pow(0.5F, delta_time * 10));
    y += (y - min + 0.1F) * blend;

    // The movement is finished
    if (y >= max) {
        y = max;
        direction = -1;
        isCrushing = false;
        crushingSound.play(0, -1);
        timer = static_cast<int>(waitDownTime);
        lastUpdate = SDL_GetTicks();
    }
}

void Crusher::applyMovement(double delta_time) {
    if (isMoving) {
        // The crusher is in a waiting state
        if (timer > 0) {
            timer -= static_cast<int>(SDL_GetTicks() - lastUpdate);
            lastUpdate = SDL_GetTicks();
        }
        // The crusher is moving
        else {
            if (direction == 1) {
                applyDownMovement(delta_time);
            } else {
                applyUpMovement(delta_time);
            }
        }
    }
}

void Crusher::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect src_rect = texture.getSize();
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, texture.getTexture(), &src_rect, &platform_rect, 0.0, nullptr, texture.getFlip());
}

void Crusher::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 249, 190, 152, 255);
    SDL_FRect platform_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &platform_rect);
}