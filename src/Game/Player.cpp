#include "../../include/Game/Player.h"

/**
 * @brief Implements the Player class representing a player in a 2D game.
 */

Player::Player(float startX, float startY, float playerSpeed, float playerWidth, float playerHeight)
        : x(startX), y(startY), speed(playerSpeed), width(playerWidth), height(playerHeight) {}

std::vector<Point> Player::getVertices() const {
    // Return the vertices of the player's bounding box.
    return {
            {x, y},
            {x + width, y},
            {x + width, y + height},
            {x, y + height}
    };
}
