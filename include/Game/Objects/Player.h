#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
#include <SDL_rect.h>
#include "../Point.h"

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
    /** CONSTRUCTOR **/

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


    /** BASIC ACCESSORS **/

    /**
     * @brief Return the x attribute.
     * @return The value of the x attribute
     */
    [[nodiscard]] float getX() const;

    /**
     * @brief Return the y attribute.
     * @return The value of the y attribute
     */
    [[nodiscard]] float getY() const;

    /**
     * @brief Return the weight attribute.
     * @return The value of the weight attribute
     */
    [[nodiscard]] float getW() const;

    /**
     * @brief Return the height attribute.
     * @return The value of the height attribute
     */
    [[nodiscard]] float getH() const;

    /**
     * @brief Return the speed attribute.
     * @return The value of the speed attribute
     */
    [[nodiscard]] float getSpeed() const;

    /**
     * @brief Return the moveX attribute.
     * @return The value of the moveX attribute
     */
    [[nodiscard]] float getMoveX() const;

    /**
     * @brief Return the moveY attribute.
     * @return The value of the moveY attribute
     */
    [[nodiscard]] float getMoveY() const;

    /**
     * @brief Return the timeAfterFall attribute.
     * @return The value of the timeAfterFall attribute
     */
    [[nodiscard]] float getTimeAfterFall() const;

    /**
     * @brief Return the currentDirection attribute.
     * @return The value of the currentDirection attribute (-1 for left, 1 for right)
     */
    [[nodiscard]] int getCurrentDirection() const;

    /**
     * @brief Return the desiredDirection attribute.
     * @return The value of the desiredDirection attribute (-1 for left, 1 for right)
     */
    [[nodiscard]] int getDesiredDirection() const;

    /**
     * @brief Return the canMove attribute.
     * @return The value of the canMove attribute
     */
    [[nodiscard]] bool getCanMove() const;

    /**
     * @brief Return the wantToMoveRight attribute.
     * @return The value of the wantToMoveRight attribute
     */
    [[nodiscard]] bool getWantToMoveRight() const;

    /**
     * @brief Return the wantToMoveLeft attribute.
     * @return The value of the wantToMoveLeft attribute
     */
    [[nodiscard]] bool getWantToMoveLeft() const;

    /**
     * @brief Return the isOnPlatform attribute.
     * @return The value of the isOnPlatform attribute
     */
    [[nodiscard]] bool getIsOnPlatform() const;

    /**
     * @brief Return the isJumping attribute.
     * @return The value of the isJumping attribute
     */
    [[nodiscard]] bool getIsJumping() const;

    /**
     * @brief Return the wantToJump attribute.
     * @return The value of the wantToJump attribute
     */
    [[nodiscard]] bool getWantToJump() const;


    /** SPECIFIC ACCESSORS **/

    /**
     * @brief Get the vertices of the player's bounding box.
     * @return A vector of Point representing the vertices.
     */
    [[nodiscard]] std::vector<Point> getVertices() const;

    /**
     * @brief Gets the vertices of the player's bounding box, adjusted to capture the wall.
     * @return A vector of Point representing vertices, with added margin to capture wall within the area.
     */
    [[nodiscard]] std::vector<Point> getVerticesHorizontal() const;

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

    /**
     * @brief Gets the horizontal collider bounding box of the player's, according to its current direction.
     * @param direction The direction in which the player intends to move.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getHorizontalCollider() const;

    /**
     * @brief Get the bounding box of the player's ground collider.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getGroundColliderBoundingBox() const;

    /**
     * @brief Get the bounding box of the player's roof collider.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getRoofColliderBoundingBox() const;

    /**
     * @brief Get the bounding box of the player.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;

    // Equality operator for comparing two players
    bool operator==(const Player &other) const {
        return (x == other.x && y == other.y && width == other.width && height == other.height);
    }


    /** MODIFIERS **/

    /**
     * @brief Set the x attribute.
     * @param val The new value of the x attribute.
     */
    void setX(float val);

    /**
     * @brief Set the y attribute.
     * @param val The new value of the y attribute.
     */
    void setY(float val);

    /**
     * @brief Set the width attribute.
     * @param val The new value of the width attribute.
     */
    void setW(float val);

    /**
     * @brief Set the height attribute.
     * @param val The new value of the height attribute.
     */
    void setH(float val);

    /**
     * @brief Set the moveX attribute.
     * @param val The new value of the moveX attribute.
     */
    void setMoveX(float val);

    /**
     * @brief Set the moveY attribute.
     * @param val The new value of the moveY attribute.
     */
    void setMoveY(float val);

    /**
     * @brief Set the finishTheMovement attribute.
     * @param state The new value of the finishTheMovement attribute.
     */
    void setFinishTheMovement(bool state);

    /**
     * @brief Set the timeSpeed attribute.
     * @param state The new value of the timeSpeed attribute.
     */
    void setTimeSpeed(float val);

    /**
     * @brief Set the desiredDirection attribute.
     * @param val The new value of the desiredDirection attribute.
     */
    void setDesiredDirection(int val);

    /**
     * @brief Set the canMove attribute.
     * @param state The new value of the canMove attribute.
     */
    void setCanMove(bool state);

    /**
     * @brief Set the wantToMoveRight attribute.
     * @param state The new value of the wantToMoveRight attribute.
     */
    void setWantToMoveRight(bool state);

    /**
     * @brief Set the wantToMoveLeft attribute.
     * @param state The new value of the wantToMoveLeft attribute.
     */
    void setWantToMoveLeft(bool state);

    /**
     * @brief Set the timeSpentJumping attribute.
     * @param state The new value of the timeSpentJumping attribute.
     */
    void setTimeSpentJumping(float val);

    /**
     * @brief Set the isOnPlatform attribute.
     * @param state The new value of the isOnPlatform attribute.
     */
    void setIsOnPlatform(bool state);

    /**
     * @brief Set the wantToJump attribute.
     * @param state The new value of the wantToJump attribute.
     */
    void setWantToJump(bool state);


    /** PUBLIC METHODS **/


    /**
     * @brief Teleports the player to a specific location.
     * @param newX The X-coordinate of the location.
     * @param newY The Y-coordinate of the location.
     */
    void teleportPlayer(float newX, float newY);

    /**
     * @brief Calculate the new position of the player.
     */
    void calculatePlayerMovement();



private:
    /** ATTRIBUTES **/

    float x; /**< The x-coordinate of the player's position. (in pixels) */
    float y; /**< The y-coordinate of the player's position. */
    float speed; /**< The speed of the player. */
    float speedMax; /**< The maximum speed of the player. */
    float moveX = 0;/**< The actual speed of the player into the x axis. */
    float moveY = 0;/**< The actual speed of the player into the y axis. */
    float width; /**< The width of the player. (in pixels) */
    float height; /**< The height of the player. */

    float timeAfterFall = COYOTE_TIME; /**< The time that has elapsed since the player started to fall */

    bool finishTheMovement = true; /**< If the player has finish the movement and can change direction */
    int desiredDirection = 0;/**< The direction the player wants to go (-1 for left, 1 for right) */
    int currentDirection = 0; /**< The current direction of the player (-1 for left, 1 for right) */
    float timeSpeed = 0; /**< The time that has elapsed since the player started running */
    float maxSpeedReachWithThisTime = 5.2F; /**< the speed max is reach at this time */
    bool canMove = true; /**< If the player can move */
    bool wantToMoveRight = false; /**< If the player try to move right */
    bool wantToMoveLeft = false; /**< If the player try to move left */


    float timeSpentJumping = 0.; /**< The time that has elapsed since the player started jumping */
    bool isOnPlatform = false; /**< If the player is on a platform */
    bool isJumping = false; /**< If the player is jumping */
    bool wantToJump = false; /**< If the player try to jump */


    /** PRIVATE METHODS **/

    /**
     * @brief Get the bounding box of the player's left collider.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getLeftColliderBoundingBox() const;

    /**
     * @brief Get the bounding box of the player's right collider.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getRightColliderBoundingBox() const;

};

#endif //PLAY_TOGETHER_PLAYER_H
