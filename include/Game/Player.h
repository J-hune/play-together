#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
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
    const int RIGHT = 1;/**< Constant to the direction of the player here right*/
    const int LEFT = -1;/**< Constant to the direction of the player here left*/

    const float LIMIT_TIME_JUMP = 6;

    int x; /**< The x-coordinate of the player's position. (in pixels) */
    int y; /**< The y-coordinate of the player's position. */
    int speed; /**< The speed of the player. */
    int speedMax;/**< The maximum speed of the player. */
    int width; /**< The width of the player. (in pixels) */
    int height; /**< The height of the player. */

    bool canMove = true; /**< If the player can move */
    bool wantMove = false; /**< If the player try to move */

    float timeJump=0.; /**<The time that has elapsed since the player started jumping */
    bool isOnPlatform = false; /**< If the player is on a platform */
    bool isJumping = false;/**< If the player is in a jump */
    bool wantToJump = false;/**< If the player try to jump */

    /**
     * @brief Constructor for the Player class.
     * @param startX Initial x-coordinate of the player.
     * @param startY Initial y-coordinate of the player.
     * @param playerSpeed Speed of the player.
     * @param speedMax Maximum player speed.
     * @param playerWidth Width of the player.
     * @param playerHeight Height of the player.
     */
    Player(int startX, int startY, int playerSpeed, int speedMax, int playerWidth, int playerHeight);

    /**
     * @brief Get the vertices of the player's bounding box.
     * @return A vector of Point representing the vertices.
     */
    [[nodiscard]] std::vector<Point> getVertices() const;

    /**
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the wall.
     * @param direction The direction in which the player intends to move.
     * @return A vector of Point representing vertices, with added margin to capture wall within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesHorizontal(int direction) const;

    /**
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the ground.
     * @return A vector of Point representing vertices, with added margin to capture ground within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesGround() const;

    /**
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the roof.
     * @return A vector of Point representing vertices, with added margin to capture roof within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesRoof() const;
};

#endif //PLAY_TOGETHER_PLAYER_H
