#include "../../include/Game/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */

Player::Player(int startX, int startY, int playerSpeed, int playerWidth, int playerHeight)
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
