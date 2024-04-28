#include "../../../include/Game/Items/Item.h"

/**
 * @file Item.cpp
 * @brief Implements the Item class responsible for Item object.
 */


/* CONSTRUCTORS */

Item::Item(float X, float Y, float height, float width, const std::string& file_name) :
            x(X), y(Y), width(width), height(height), collectSound("Items/" + file_name) {}


/* ACCESSORS */

float Item::getX() const {
    return x;
}

float Item::getY() const {
    return y;
}

float Item::getHeight() const {
    return height;
}

float Item::getWidth() const {
    return  width;
}

SDL_FRect Item::getBoundingBox() const {
    return {x, y, width, height};
}


bool Item::operator==(const Item &item) const {
     return x == item.getX()
            && y == item.getY()
            && width == item.getWidth()
            && height == item.getHeight();
}


/* METHODS */

/*void Item::applyEffect(Player &player) {
    player.addToScore(5);
    collectSound.play(0, -1);
}*/

void Item::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_FRect itemRect = {x - camera.x, y - camera.y, width, height};
    SDL_RenderFillRectF(renderer, &itemRect);
}
