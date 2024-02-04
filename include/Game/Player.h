#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
#include <SDL_rect.h>
#include "Point.h"

/**
 * @file Player.h
 * @brief Defines the Player class representing a player in a 2D game.
 */

/**
 * @class Player
 * @brief Represents a player in a 2D game with position, speed, and dimensions.
 */

class Player {
public:
    int x; /**< The x-coordinate of the player's position. (in pixels) */
    int y; /**< The y-coordinate of the player's position. */
    int speed; /**< The speed of the player. */
    int width; /**< The width of the player. (in pixels) */
    int height; /**< The height of the player. */

    /**
     * @brief Constructor for the Player class.
     * @param startX Initial x-coordinate of the player.
     * @param startY Initial y-coordinate of the player.
     * @param playerSpeed Speed of the player.
     * @param playerWidth Width of the player.
     * @param playerHeight Height of the player.
     */
    Player(int startX, int startY, int playerSpeed, int playerWidth, int playerHeight);

    /**
     * @brief Get the vertices of the player's bounding box.
     * @return A vector of Point representing the vertices.
     */
    [[nodiscard]] std::vector<Point> getVertices() const;

    [[nodiscard]] SDL_Rect getBoundingBox() const {
        SDL_Rect boundingBox = {x, y, width, height};
        return boundingBox;
    }

    // Equality operator for comparing two players
    bool operator==(const Player &other) const {
        return (x == other.x && y == other.y && width == other.width && height == other.height);
    }
};

#endif //PLAY_TOGETHER_PLAYER_H
