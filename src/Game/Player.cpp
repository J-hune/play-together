#include "../../include/Game/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */

Player::Player(float startX, float startY, float playerSpeed,float speedMax, float playerWidth, float playerHeight)
        : x(startX), y(startY), speed(playerSpeed), speedMax(speedMax), width(playerWidth), height(playerHeight){}

std::vector<Point> Player::getVertices() const {
    // Return the vertices of the player's bounding box.
    return {
            {x, y},
            {x + width, y},
            {x + width, y + height},
            {x, y + height}
    };
}

std::vector<Point> Player::getVerticesHorizontal(int direction) const {
    // Return the vertices of the player's bounding box, with added margin to capture wall within the area.

    std::vector<Point> verticesDirection;

    if(direction==PLAYER_LEFT){
        verticesDirection = getVerticesLeft();
    }
    else{
        verticesDirection = getVerticesRight();
    }

    return verticesDirection;
}

std::vector<Point> Player::getVerticesLeft() const {
    // Return the vertices of the player's bounding box, with added margin to capture left wall within the area.
    return {
            {x + (- 2),         y + 2},
            {x + ( 2), y + 2},
            {x + ( 2), y + height - 2},
            {x + (- 2),         y + height - 2}
    };
}

std::vector<Point> Player::getVerticesRight() const {
    // Return the vertices of the player's bounding box, with added margin to capture right wall within the area.
    return {
            {width + x + (2),         y + 2},
            {width + x + (-2), y + 2},
            {width + x + (-2), y + height - 2},
            {width + x + (2),         y + height - 2}
    };
}
std::vector<Point> Player::getVerticesGround() const {
    // Return the vertices of the player's bounding box, with added margin to capture ground within the area.
    return {
            {x + 2,         y + height},
            {x + width - 2, y + height},
            {x + width - 2, y + height + 2},
            {x + 2,         y + height + 2}
    };
}
std::vector<Point> Player::getVerticesRoof() const {
    // Return the vertices of the player's bounding box, with added margin to capture roof within the area.
    return {
            {x + 2,         y + 2},
            {x + width - 2, y + 2},
            {x + width - 2,  y },
            {x + 2,         y }
    };
}