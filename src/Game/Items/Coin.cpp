#include "../../../include/Game/Items/Coin.h"

/**
 * @class Coin
 * @brief Represents the coin object.
 */

// Initialize texture pointers
SDL_Texture *Coin::baseSpriteTexturePtr = nullptr;
Sprite Coin::sprite;

Coin::Coin(float x, float y, float w, float h, int value) : Item(x, y, w, h), value(value) {
    spritePtr = &sprite;
}


/* BASIC ACCESSORS **/

int Coin::getValue() const {
    return value;
}

void Coin::setValue(int newValue) {
    value = newValue;
}

void Coin::setAnimation(Animation newAnimation) {
    sprite.setAnimation(newAnimation);
}

/* METHODS */

void Coin::applyEffect(Player &player) const {
    player.addToScore(value);
}


bool Coin::loadTextures(SDL_Renderer &renderer) {
    // Load players' sprite texture
    baseSpriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/items/coins.png");

    // Check errors
    if (baseSpriteTexturePtr == nullptr) {
        return false; // Return failure
    }
    sprite = Sprite(Coin::gold,*baseSpriteTexturePtr,16,16);
    return true; // Return success
}

void Coin::updateSprite() {
    sprite.updateAnimation(); // Update sprite animation
}

void Coin::render(SDL_Renderer *renderer, Point camera) const {
    SDL_Rect srcRect = (*spritePtr).getSrcRect();
    SDL_FRect itemRect = {getX() - camera.x, getY() - camera.y, getWidth(), getHeight()};
    SDL_RenderCopyExF(renderer, (*spritePtr).getTexture(), &srcRect, &itemRect, 0.0, nullptr, (*spritePtr).getFlip());
}