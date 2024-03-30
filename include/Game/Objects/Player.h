#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include "../Point.h"
#include "../../Graphics/Animation.h"
#include "../../Graphics/Sprite.h"

const int PLAYER_RIGHT = 1; /**< Constant for the player's right direction. */
const int PLAYER_LEFT = -1; /**< Constant for the player's left direction. */

const float PRESSURE_JUMP_MIN = 0.25F; /**< Constant for the jump time limit minimum. */
const float PRESSURE_JUMP_MAX = 0.90F; /**< Constant for jump time limit. */

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
    /** PUBLIC STATIC ATTRIBUTES **/

    static constexpr Animation idle = {0, 4, 100}; /**< Idle animation */
    static constexpr Animation walk = {1, 6, 70}; /**< Walk animation */
    static constexpr Animation hit = {2, 3, 10}; /**< Hit animation */
    static constexpr Animation hurt = {3, 4, 100}; /**< Hurt animation */
    static constexpr Animation run = {4, 7, 100}; /**< Run animation */


    /** CONSTRUCTORS **/

    Player() = default; // Used for the useless empty Game constructor of Athena

    /**
     * @brief Constructor for the Player class.
     * @param startX Initial x-coordinate of the player.
     * @param startY Initial y-coordinate of the player.
     * @param playerSpeed Speed of the player.
     * @param speedMax Maximum player speed.
     * @param playerWidth Width of the player.
     * @param playerHeight Height of the player.
     */
    Player(float startX, float startY, float playerSpeed, float playerWidth, float playerHeight);


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
     * @brief Return the sprite attribute.
     * @return A pointer of a sprite object representing the sprite of the player.
     */
    [[nodiscard]] Sprite* getSprite();

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
     * @brief Return the currentDirection attribute.
     * @return The value of the currentDirection attribute (-1 for left, 1 for right)
     */
    [[nodiscard]] int getDirectionX() const;

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
     * @brief Return the mavity attribute.
     * @return The value of the mavity attribute
     */
    [[nodiscard]] float getMavity() const;

    /**
     * @brief Return the wantToJump attribute.
     * @return The value of the wantToJump attribute
     */
    [[nodiscard]] bool getWantToJump() const;

    /**
     * @brief Return the currentDirection attribute.
     * @return The value of the currentDirection attribute (-1 for left, 1 for right)
     */
    [[nodiscard]] int getDirectionY() const;

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


    /** SPECIFIC ACCESSORS **/

    /**
     * @brief Gets the vertices of the player's bounding box.
     * @return A vector of Point representing the vertices.
     */
    [[nodiscard]] std::vector<Point> getVertices() const;

    /**
     * @brief Gets the vertices of the player's bounding box of the next frame.
     * @return A vector of Point representing the vertices of the next frame.
     */
    [[nodiscard]] std::vector<Point> getVerticesNextFrame() const;

    /**
     * @brief Gets the vertices of the player's horizontal collider, based on its current direction.
     * @return A vector of Point representing the vertices.
     * @see getLeftColliderVertices() and getRightColliderVertices() for detailed usage.
     */
    [[nodiscard]] std::vector<Point> getHorizontalColliderVertices() const;

    /**
     * @brief Gets the vertices of the player's ground collider.
     * @return A vector of Point representing the vertices.
     * @see getGroundColliderBoundingBox() to get the bounding box of the ground collider.
     */
    [[nodiscard]] std::vector<Point> getGroundColliderVertices() const;

    /**
     * @brief Gets the vertices of the player's roof collider.
     * @return A vector of Point representing the vertices.
     * @see getRoofColliderBoundingBox() to get the bounding box of the roof collider.
     */
    [[nodiscard]] std::vector<Point> getRoofColliderVertices() const;

    /**
     * @brief Gets the horizontal collider bounding box of the player's, based on its current direction.
     * @return SDL_Rect representing the bounding box.
     * @see getLeftColliderBoundingBox() and getRightColliderBoundingBox() for detailed usage.
     */
    [[nodiscard]] SDL_FRect getHorizontalColliderBoundingBox() const;

    /**
     * @brief Gets the bounding box of the player's ground collider.
     * @return SDL_Rect representing the bounding box.
     * @see getGroundColliderVertices() to get the vertices of the ground collider.
     */
    [[nodiscard]] SDL_FRect getGroundColliderBoundingBox() const;

    /**
     * @brief Gets the bounding box of the player's roof collider.
     * @return SDL_Rect representing the bounding box.
     * @see getRoofColliderVertices() to get the vertices of the roof collider.
     */
    [[nodiscard]] SDL_FRect getRoofColliderBoundingBox() const;

    /**
     * @brief Gets the bounding box of the player.
     * @return SDL_Rect representing the bounding box.
     * @see getVertices() to get the vertices of the player.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;

    /**
     * @brief Gets the bounding box of the player of the next frame.
     * @return SDL_Rect representing the bounding box of the next frame.
     * @see getVertices() to get the vertices of the player this frame.
     */
    [[nodiscard]] SDL_FRect getBoundingBoxNextFrame() const;

    // Equality operator for comparing two players
    bool operator==(const Player &other) const {
        return (x == other.x && y == other.y && width == other.width && height == other.height);
    }


    /** MODIFIERS **/

    /**
     * @brief Sets the x attribute.
     * @param val The new value of the x attribute.
     */
    void setX(float val);

    /**
     * @brief Sets the y attribute.
     * @param val The new value of the y attribute.
     */
    void setY(float val);

    /**
     * @brief Sets the width attribute.
     * @param val The new value of the width attribute.
     */
    void setW(float val);

    /**
     * @brief Sets the height attribute.
     * @param val The new value of the height attribute.
     */
    void setH(float val);

    /**
     * @brief Sets the moveX attribute.
     * @param val The new value of the moveX attribute.
     */
    void setMoveX(float val);

    /**
     * @brief Sets the moveY attribute.
     * @param val The new value of the moveY attribute.
     */
    void setMoveY(float val);

    /**
     * @brief Sets the canMove attribute.
     * @param state The new value of the canMove attribute.
     */
    void setCanMove(bool state);

    /**
     * @brief Sets the wantToMoveRight attribute.
     * @param state The new value of the wantToMoveRight attribute.
     */
    void setWantToMoveRight(bool state);

    /**
     * @brief Sets the wantToMoveLeft attribute.
     * @param state The new value of the wantToMoveLeft attribute.
     */
    void setWantToMoveLeft(bool state);

    /**
     * @brief Sets the isOnPlatform attribute.
     * @param state The new value of the isOnPlatform attribute.
     */
    void setIsOnPlatform(bool state);

    /**
     * @brief Sets the wantToJump attribute.
     * @param state The new value of the wantToJump attribute.
     */
    void setWantToJump(bool state);

    /**
     * @brief Toggle the gravity to positive or negative (normal or reversed).
     */
    void toggleMavity();

    /**
     * @brief Sets the timeSpentJumping attribute.
     * @param val The new value of timeSpentJumping.
     */
    void setTimeSpentJumping(float val);


    /** PUBLIC METHODS **/

    /**
     * @brief Load all players textures.
     * @param renderer The renderer of the game.
     * @return Returns true if all textures were loaded correctly, false otherwise.
     */
    static bool loadTextures(SDL_Renderer &renderer);

    /**
     * @brief Assign a texture to a player's sprite according to the id.
     * @param id The id of the texture.
     */
    void setSpriteTextureByID(int id);

    /**
     * @brief Teleports the player to a specific location.
     * @param newX The X-coordinate of the location.
     * @param newY The Y-coordinate of the location.
     */
    void teleportPlayer(float newX, float newY);

    /**
     * @brief Calculate the new position of the player.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateXaxisMovement() and calculateYaxisMovement() for sub-functions.
     */
    void calculateMovement(float deltaTime);

    /**
     * @brief Apply the movement by adding moveX and moveY to the player position.
     */
    void applyMovement();

    /**
     * @brief Renders the player's sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera);

    /**
     * @brief Renders the player's box, used for debugging.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the player's colliders, used for debugging.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderColliders(SDL_Renderer *renderer, Point camera) const;


private:
    /** ATTRIBUTES **/

    // CHARACTERISTIC ATTRIBUTES
    float x; /**< The x-coordinate of the player's position. (in pixels) */
    float y; /**< The y-coordinate of the player's position. */
    float speed; /**< The speed of the player. */
    float width; /**< The width of the player. (in pixels) */
    float height; /**< The height of the player. */
    Sprite sprite; /**< The sprite of the player. */

    // X-AXIS MOVEMENT ATTRIBUTES
    float moveX = 0; /**< Player movement on x-axis during 'this' frame. */
    bool wantToMoveRight = false; /**< If the player pressed a key to move right */
    bool wantToMoveLeft = false; /**< If the player pressed a key to move left */
    float directionX = 0; /**< The current direction of the player (-1 for left, 1 for right) */
    float previousDirectionX = 0; /**< The direction of the player on the x-axis during the previous frame */
    bool canMove = true; /**< If the player can move */
    float baseMovementX = 500; /**< The base movement speed of the player on the x-axis, independent of acceleration or deceleration factors. */
    float initialSpeedCurveX = 0.3f; /**< The initial speed curve factor used for acceleration when the player starts moving. A lower value results in slower initial acceleration. */
    float accelerationFactorX = 4.0f; /**< The acceleration factor controlling how quickly the player's speed curve increases when moving. A higher value results in faster acceleration. */
    float decelerationFactorX = 3.0f; /**< The deceleration factor controlling how quickly the player's speed curve decreases when not moving. A higher value results in faster deceleration. */
    float speedCurveX = 1; /**< The speed curve attribute that is multiplied to player's x-axis movement */

    // Y-AXIS MOVEMENT ATTRIBUTES
    float moveY = 0; /**< player movement on y-axis during 'this' frame. */
    float mavity = 1;
    bool wantToJump = false; /**< If the player pressed a key to jump */
    float directionY = 0; /**< The current direction of the player (-1 for up, 1 for down) */
    bool isOnPlatform = false; /**< If the player is on a platform */
    bool isJumping = false; /**< If the player is jumping */
    Uint32 lastTimeOnPlatform = SDL_GetTicks(); /**< The last time the player was on a platform (also the time when he started jumping). */
    float coyoteTime = 2; /**< The time in seconds the player can jump after starting falling. */
    float timeSpentJumping = 0;


    // LOADED TEXTURES IN STATIC ATTRIBUTES
    static SDL_Texture *baseSpriteTexturePtr; /**< The base texture of a player */
    static SDL_Texture *spriteTexture1Ptr; /**< The texture 1 of players */
    static SDL_Texture *spriteTexture2Ptr; /**< The texture 2 of players */
    static SDL_Texture *spriteTexture3Ptr; /**< The texture 3 of players */
    static SDL_Texture *spriteTexture4Ptr; /**< The texture 4 of players */


    /** PRIVATE METHODS **/

    /**
     * @brief Gets the vertices of the player's left collider.
     * @return A vector of Point representing the vertices.
     * @see getHorizontalColliderVertices() for main use.
     */
    [[nodiscard]] std::vector<Point> getLeftColliderVertices() const;

    /**
     * @brief Gets the vertices of the player's right collider.
     * @return A vector of Point representing the vertices.
     * @see getHorizontalColliderVertices() for main use.
     */
    [[nodiscard]] std::vector<Point> getRightColliderVertices() const;

    /**
     * @brief Gets the bounding box of the player's left collider.
     * @return SDL_Rect representing the bounding box.
     * @see getHorizontalColliderBoundingBox() for main use.
     */
    [[nodiscard]] SDL_FRect getLeftColliderBoundingBox() const;

    /**
     * @brief Gets the bounding box of the player's right collider.
     * @return SDL_Rect representing the bounding box.
     * @see getHorizontalColliderBoundingBox() for main use.
     */
    [[nodiscard]] SDL_FRect getRightColliderBoundingBox() const;

    /**
     * @brief Checks if the player can jump by checking coyote time and if he is on a platform.
     * @return Returns true if the player can jump, false otherwise.
     */
    bool canJump();

    /**
     * @brief To be written.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateMovement() for main use.
     * TODO: correct the speed curve issue (linked to the fps limitation)
     */
    void calculateXaxisMovement(float deltaTime);

    /**
     * @brief To be written.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateMovement() for main use.
     * TODO: correct the jump
     */
    void calculateYaxisMovement(float deltaTime);

};

#endif //PLAY_TOGETHER_PLAYER_H
