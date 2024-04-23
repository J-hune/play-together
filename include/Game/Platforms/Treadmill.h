#ifndef PLAY_TOGETHER_TREADMILL_H
#define PLAY_TOGETHER_TREADMILL_H

#include "IPlatform.h"
#include "../../Graphics/Sprite.h"


/**
 * @file Treadmill.h
 * @brief Defines the Treadmill class representing a treadmill on x-axis.
 */

/**
 * @class Treadmill
 * @brief Represents a treadmill on an axis in a 2D game.
 */
class Treadmill {
private:
    /* ATTRIBUTES */

    // CHARACTERISTIC ATTRIBUTES
    float x; /**< The x-coordinate of the treadmill's position. */
    float y; /**< The y-coordinate of the treadmill's position. */
    float w; /**< The width of the treadmill. (in pixels) */
    float h; /**< The height of the treadmill. */
    float size; /**< The size of the treadmill. */
    float speed; /**< The speed of the treadmill. */
    float direction; /**< The current direction of the treadmill : -1 for left, 1 for right. */
    float move = 0; /**< The distance the player will move on the treadmill. */
    bool isMoving = true; /** Flag indicating if the platform is currently moving. */
    bool isOnScreen = false; /**< Flag indicating if the treadmill is on screen. */

    // SPRITE ATTRIBUTES
    static SDL_Texture *spriteTexturePtr; /**< The texture of treadmill. */
    static int SPRITE_WIDTH; /**< The width of the treadmill sprite. */
    static int SPRITE_HEIGHT; /**< The height of the treadmill sprite. */
    Uint32 spriteSpeed = 75; /**< The speed of the treadmill sprite. */
    Sprite sprite; /**< The sprite of the treadmill. */
    Animation right = {0, FRAME_NUMBER, spriteSpeed, false}; /**< Idle animation */
    Animation left = {1, FRAME_NUMBER, spriteSpeed, false}; /**< Idle animation */
    static constexpr int FRAME_NUMBER = 4; /**< Number of frames in the sprite. */

public:

    /* CONSTRUCTORS */

    Treadmill(float x, float y, float size, float speed, float direction, Uint32 spriteSpeed);


    /* ACCESSORS */

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
     * @biref Return the speed attribute.
     * @return The value of the speed attribute.
     */
    [[nodiscard]] float getSpeed() const;

    /**
     * @brief Return the direction attribute.
     * @return The value of the direction attribute.
     */
    [[nodiscard]] float getDirection() const;

    /**
     * @brief Return the move attribute.
     * @return The value of the move attribute.
     */
    [[nodiscard]] float getMove() const;

    /**
     * @brief Return the isMoving attribute.
     * @return The value of the isMoving attribute.
     */
    [[nodiscard]] bool getIsMoving() const;

    /**
     * @brief Get the bounding box of the platform.
     * @return SDL_Rect representing the platform box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;


    /* MODIFIERS */

    /**
     * @brief Set the direction attribute.
     * @param new_direction The new direction of the treadmill.
     */
    void setDirection(float new_direction);

    /**
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state);

    /**
     * @brief Set the isOnScreen attribute.
     * @param state The new state of the isOnScreen attribute.
     */
    void setIsOnScreen(bool state);

    /**
     * @brief Set the texture of the treadmill.
     * @param texturePtr The new texture of the treadmill.
     */
    static void setTexture(SDL_Texture *texturePtr);


    /* METHODS */

    /**
     * @brief Calculate movement of the player on the treadmill.
     * @param delta_time Represents the time elapsed since the last frame.
     */
    void calculateMovement(double delta_time);

    /**
     * @brief Renders the treadmill by drawing its sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera);

    /**
     * @brief Renders the treadmill by its drawing its collision box.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;

};


#endif //PLAY_TOGETHER_TREADMILL_H
