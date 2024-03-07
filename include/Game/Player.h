#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
#include <SDL_rect.h>
#include "Point.h"

const int PLAYER_RIGHT = 1; /**< Constant to the direction of the player here right. */
const int PLAYER_LEFT = -1; /**< Constant to the direction of the player here left. */

const float PRESSURE_JUMP_MIN = 4; /**< Constant for the jump time limit minimum. */
const float PRESSURE_JUMP_MAX = 7; /**< Constant for jump time limit. */
constexpr float COYOTE_TIME = 2; /**< Time allowed for jumping after a fall. */

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
    float x; /**< The x-coordinate of the player's position. (in pixels) */
    float y; /**< The y-coordinate of the player's position. */
    float speed; /**< The speed of the player. */
    float speedMax; /**< The maximum speed of the player. */
    float width; /**< The width of the player. (in pixels) */
    float height; /**< The height of the player. */

    float timeAfterFall = COYOTE_TIME; /**< The time that has elapsed since the player started to fall */

    bool finishTheMovement; /**< If the player has finish the movement and can change direction*/
    int currentDirection; /**< The current direction of the player*/
    float timeSpeed; /**< The time that has elapsed since the player started running */
    float maxSpeedReachWithThisTime = 5.2F;/**< the speed max is reach at this time */
    bool canMove = true; /**< If the player can move */
    bool wantToMoveRight = false; /**< If the player try to move right */
    bool wantToMoveLeft = false; /**< If the player try to move left */


    float timeSpentJumping = 0.; /**< The time that has elapsed since the player started jumping */
    bool isOnPlatform = false; /**< If the player is on a platform */
    bool isJumping = false; /**< If the player is jumping */
    bool wantToJump = false; /**< If the player try to jump */

    /**
     * @brief Constructor for the Player class.
     * @param startX Initial x-coordinate of the player.
     * @param startY Initial y-coordinate of the player.
     * @param playerSpeed Speed of the player.
     * @param speedMax Maximum player speed.
     * @param playerWidth Width of the player.
     * @param playerHeight Height of the player.
     */
    Player(float startX, float startY, float playerSpeed, float speedMax, float playerWidth, float playerHeight);

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
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the left wall.
     * @return A vector of Point representing vertices, with added margin to capture left wall within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesLeft() const;

    /**
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the right wall.
     * @return A vector of Point representing vertices, with added margin to capture the right wall within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesRight() const;

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

    [[nodiscard]] SDL_FRect getBoundingBox() const {
        SDL_FRect boundingBox = {x, y, width, height};
        return boundingBox;
    }

    // Equality operator for comparing two players
    bool operator==(const Player &other) const {
        return (x == other.x && y == other.y && width == other.width && height == other.height);
    }
};

#endif //PLAY_TOGETHER_PLAYER_H
