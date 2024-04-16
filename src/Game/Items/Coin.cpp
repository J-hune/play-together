#include "../../../include/Game/Items/Coin.h"

/**
 * @class Coin
 * @brief Represents the coin object.
 */

// Initialize texture pointers
SDL_Texture *Coin::spriteTexturePtr = nullptr;
Sprite Coin::sprite;


/* CONSTRUCTORS */

Coin::Coin(float x, float y, float w, float h, int value) : Item(x, y, w, h, "coin.wav"), value(value) {
    spritePtr = &sprite;
}


/* ACCESSORS */

int Coin::getValue() const {
    return value;
}


/* MUTATORS */

void Coin::setValue(int newValue) {
    value = newValue;
}


/* METHODS */

bool Coin::loadTexture(SDL_Renderer &renderer) {
    // Load players' sprite texture
    spriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/items/coins.png");

    // Check errors
    if (spriteTexturePtr == nullptr) {
        return false; // Return failure
    }
    sprite = Sprite(*spriteTexturePtr, Coin::gold, 16, 16);
    return true; // Return success
}

void Coin::applyEffect(Player &player) {
    Item::applyEffect(player);
    player.addToScore(value);
}

void Coin::render(SDL_Renderer *renderer, Point camera) {
    sprite.updateAnimation();
    SDL_Rect srcRect = (*spritePtr).getSrcRect();
    SDL_FRect itemRect = {getX() - camera.x, getY() - camera.y, getWidth(), getHeight()};
    SDL_RenderCopyExF(renderer, (*spritePtr).getTexture(), &srcRect, &itemRect, 0.0, nullptr, (*spritePtr).getFlip());
}