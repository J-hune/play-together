#ifndef PLAY_TOGETHER_PLAYER_H
#define PLAY_TOGETHER_PLAYER_H

#include <vector>
#include <SDL_image.h>
#include <map>
#include "Point.h"
#include "../Graphics/Animation.h"
#include "../Graphics/Sprite.h"

/**
 * @file Player.h
 * @brief Defines the Player class representing a player in a 2D game.
 */

/**
 * @class Player
 * @brief Represents a player in a 2D game with position, speed, and dimensions.
 */
class Player {
private:
    /* ATTRIBUTES */

    int playerID; /**< The ID of the player */
    short SpriteID = -1; /**< The character ID (sprite) of the player */

    // CHARACTERISTIC ATTRIBUTES
    float x; /**< The x-coordinate of the player's position. (in pixels) */
    float y; /**< The y-coordinate of the player's position. */
    float width; /**< The width of the player. (in pixels) */
    float height; /**< The height of the player. */
    float size = 2; /**< The size of the player. */
    int score = 0; /**< The score of the player. */

    // SPRITE ATTRIBUTES
    Sprite sprite; /**< The sprite of the player. */
    SDL_Texture  *defaultTexture = nullptr;
    SDL_Texture  *medalTexture = nullptr;

    // X-AXIS MOVEMENT ATTRIBUTES
    float moveX = 0; /**< Player movement on x-axis during 'this' frame. */
    bool wantToMoveRight = false; /**< If the player pressed a key to move right */
    bool wantToMoveLeft = false; /**< If the player pressed a key to move left */
    float directionX = 0; /**< The current direction of the player (-1 for left, 1 for right, 0 for not moving) */
    float previousDirectionX = 0; /**< The direction of the player on the x-axis during the previous frame */
    bool canMove = true; /**< If the player can move */
    float speed = 1;
    float baseMovementX = 500; /**< The base movement speed of the player on the x-axis, independent of acceleration or deceleration factors. */
    float initialSpeedCurveX = 0.3f; /**< The initial speed curve factor used for acceleration when the player starts moving. A lower value results in slower initial acceleration. */
    float accelerationFactorX = 4.0f; /**< The acceleration factor controlling how quickly the player's speed curve increases when moving. A higher value results in faster acceleration. */
    float decelerationFactorX = 3.0f; /**< The deceleration factor controlling how quickly the player's speed curve decreases when not moving. A higher value results in faster deceleration. */
    float speedCurveX = 1; /**< The speed curve attribute that is multiplied to player's x-axis movement */
    float sprintMultiplier = 1.0f; /**< The factor to adjust the player's speed when sprinting. */

    // Y-AXIS MOVEMENT ATTRIBUTES
    float moveY = 0; /**< Vertical movement of the player during this frame. */
    float mavity = 30.f; /**< Gravity acceleration. */
    bool wantToJump = false; /**< Flag indicating whether the player has requested to jump. */
    bool jumpLock = false; /**< Flag indicating whether the player has already jumped. */
    float directionY = 0; /**< Current vertical direction of the player (-1 for up, 1 for down, 0 for no movement). */
    bool isOnPlatform = false; /**< Flag indicating whether the player is currently on a platform. */
    bool isJumping = false; /**< Flag indicating whether the player is currently in a jump. */
    Uint32 lastTimeOnPlatform = SDL_GetTicks(); /**< Timestamp of the last time the player was on a platform. */
    float jumpInitialVelocity = 525.f; /**< Initial velocity of the player's jump. */
    float jumpMaxDuration = 0.2f; /**< Maximum duration of the player's jump. */
    float maxFallSpeed = 600.f; /**< Maximum falling speed of the player. */
    float fallSpeedFactor = 75.0f; /**< Factor to adjust the player's fall speed. */
    float coyoteTime = 0.15f; /**< Time window in seconds during which the player can still jump after starting to fall. */
    Uint64 jumpStartTime = 0; /**< Timestamp of the start of the player's jump. */
    float jumpVelocity = 0; /**< Current velocity of the player's jump. */
    size_t currentZoneID = 0; /**< The ID of the current zone the player is in. */

    // LOADED TEXTURES
    static SDL_Texture *baseSpriteTexturePtr; /**< The base texture of a player */
    static SDL_Texture *spriteTexture1Ptr; /**< The texture 1 of players */
    static SDL_Texture *spriteTexture2Ptr; /**< The texture 2 of players */
    static SDL_Texture *spriteTexture3Ptr; /**< The texture 3 of players */
    static SDL_Texture *spriteTexture4Ptr; /**< The texture 4 of players */
    static SDL_Texture *spriteTexture1MedalPtr;/**< The medal's texture 1 of players */
    static SDL_Texture *spriteTexture2MedalPtr;/**< The medal's texture 2 of players */
    static SDL_Texture *spriteTexture3MedalPtr;/**< The medal's texture 3 of players */
    static SDL_Texture *spriteTexture4MedalPtr;/**< The medal's texture 4 of players */

    // SPRITE ANIMATIONS
    static constexpr Animation idle = {0, 4, 100, false}; /**< Idle animation */
    static constexpr Animation sneak = {4, 1, 1000000000, false}; /**< Sneak animation */
    static constexpr Animation walk = {1, 6, 70, false}; /**< Walk animation */
    static constexpr Animation hit = {2, 3, 10, true}; /**< Hit animation */
    static constexpr Animation hurt = {3, 4, 100, true}; /**< Hurt animation */
    static constexpr Animation run = {4, 7, 100, false}; /**< Run animation */

    // CONSTANTS
    static constexpr int PLAYER_RIGHT = 1; /**< Constant for the player's right direction. */
    static constexpr int PLAYER_LEFT = -1; /**< Constant for the player's left direction. */
    static constexpr int BASE_WIDTH = 24; /**< Constant for the base width of a player. */
    static constexpr int BASE_HEIGHT = 18; /**< Constant for the base height of a player. */


public:

    /* CONSTRUCTORS */

    /**
     * @brief Constructor for the Player class.
     * @param spawnPoint The spawn point of the player.
     * @param playerWidth Width of the player.
     * @param playerHeight Height of the player.
     */
    Player(int playerID, Point spawnPoint, float size);


    /* BASIC ACCESSORS */

    /**
     * @brief Return the player ID.
     * @return The player ID.
     */
    [[nodiscard]] int getPlayerID() const;

    /**
     * @brief Return the sprite ID of the character.
     * @return The sprite ID of the character.
     */
    [[nodiscard]] short getSpriteID() const;

    /**
     * @brief Return the x attribute.
     * @return The value of the x attribute.
     */
    [[nodiscard]] float getX() const;

    /**
     * @brief Return the y attribute.
     * @return The value of the y attribute.
     */
    [[nodiscard]] float getY() const;

    /**
     * @brief Return the weight attribute.
     * @return The value of the weight attribute.
     */
    [[nodiscard]] float getW() const;

    /**
     * @brief Return the height attribute.
     * @return The value of the height attribute.
     */
    [[nodiscard]] float getH() const;

    /**
     * @brief Return the size attribute.
     * @return The value of the size attribute.
     */
    [[nodiscard]] float getSize() const;

    /**
     * @brief Return the sprite attribute.
     * @return A pointer of a sprite object representing the sprite of the player.
     */
    [[nodiscard]] Sprite* getSprite();

    /**
     * @brief Return the moveX attribute.
     * @return The value of the moveX attribute.
     */
    [[nodiscard]] float getMoveX() const;

    /**
     * @brief Return the moveY attribute.
     * @return The value of the moveY attribute.
     */
    [[nodiscard]] float getMoveY() const;

    /**
     * @brief Return the currentDirection attribute.
     * @return The value of the currentDirection attribute (-1 for left, 1 for right)
     */
    [[nodiscard]] int getDirectionX() const;

    /**
     * @brief Return the canMove attribute.
     * @return The value of the canMove attribute.
     */
    [[nodiscard]] bool getCanMove() const;

    /**
     * @brief Return the speed attribute.
     * @return The value of the speed attribute.
     */
    [[nodiscard]] float getSpeed() const;

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

    /**
     * @brief Return the id of the current zone in which the player is.
     * @return The current zone ID.
     */
    [[nodiscard]] size_t getCurrentZoneID() const;

    /**
     * @brief Gets the player's current score.
     * @return The player's score as an integer.
     */
    [[nodiscard]] int getScore() const;

    /* SPECIFIC ACCESSORS */

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


    /* MODIFIERS */

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
     * @brief Sets the size attribute and the width and height attributes accordingly.
     * @param val The new value of the size attribute.
     */
    void setSize(float val);

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
     * @brief Sets the speed attribute.
     * @param state The new value of the speed attribute.
     */
    void setSpeed(float val);

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
     * @brief Sets player attributes according to sprint state.
     * @param state The state of the sprint, true for sprint, false for walk.
     */
    void setSprint(bool state);

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
     * @brief Sets the isJumping attribute.
     * @param state The new value of the isJumping attribute.
     */
    void setIsJumping(bool state);

    /**
     * @brief Toggle the gravity to positive or negative (normal or reversed).
     */
    void toggleMavity();

    /**
     * @brief Set the player ID.
     * @param id The player ID to set (socket file descriptor).
     */
    void setPlayerID(int id);

    /**
    * @brief Set the sprite ID of the character.
    * @param id The sprite ID to set (between 1 and 4).
    */
    void setSpriteID(short id);

    /**
     * @brief Set the current zone ID of the player.
     * @param id The zone ID to set.
     */
    void setCurrentZoneID(size_t id);

    /**
     * @brief Set the mavity attribute.
     * @param val The new value of the mavity attribute.
     */
    void setMaxFallSpeed(float val);


    /**
     * @brief Sets the default texture for the player.
     * @param newTexture The new default texture to set.
     */
    void setDefaultTexture(SDL_Texture* newTexture);


    /**
     * @brief Sets the medal texture for the player.
     * @param newTexture The new medal texture to set.
     */
    void setMedalTexture(SDL_Texture* newTexture);


    /* PUBLIC METHODS */

    /**
     * @brief Sets the player's sprite texture to the default texture.
     */
    void useDefaultTexture();

    /**
     * @brief Sets the player's sprite texture to the medal texture.
     */
    void useMedalTexture();

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
     * @brief Increase or decrease the score by adding a value.
     * @param val The value to add to the score.
     */
    void addToScore(int val);

    /**
     * @brief Calculate the new position of the player.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateXaxisMovement() and calculateYaxisMovement() for sub-functions.
     */
    void calculateMovement(double deltaTime);

    /**
     * @brief Checks if the player has moved by checking moveX and moveY attributes.
     * @return True if the player has moved, false otherwise.
     */
    [[nodiscard]] bool hasMoved() const;

    /**
     * @brief Apply the movement by adding moveX and moveY to the player position.
     * @param ratio The ratio of the movement to apply.
     */
    void applyMovement(double ratio);

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

    /* PRIVATE METHODS */

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
    [[nodiscard]] bool canJump() const;

    /**
     * @brief Calculates the horizontal movement of the player for the current frame.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateMovement() for main use.
     */
    void calculateXaxisMovement(double deltaTime);

    /**
     * @brief Calculates the vertical movement of the player for the current frame.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @see calculateMovement() for main use.
     */
    void calculateYaxisMovement(double deltaTime);

    /**
     * @brief Update the sprite animation of the player.
     */
    void updateSpriteAnimation();

    /**
     * @brief Update the sprite orientation of the player.
     * @param direction The current x-axis direction of the player.
     */
    void updateSpriteOrientation();

};

#endif //PLAY_TOGETHER_PLAYER_H
