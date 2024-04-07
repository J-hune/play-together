#ifndef PLAY_TOGETHER_MOVINGPLATFORM2D_H
#define PLAY_TOGETHER_MOVINGPLATFORM2D_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>
#include "../Point.h"

/**
 * @file MovingPlatform2D.h
 * @brief Defines the MovingPlatform2D class representing a moving platform on x-axis and y-axis.
 */

/**
 * @class MovingPlatform2D
 * @brief Represents a moving platform on two axis in a 2D game.
 */
class MovingPlatform2D {
public:
    /** CONSTRUCTOR **/

    MovingPlatform2D(float x, float y, float w, float h, float speed, Point left, Point right, bool start);


    /** BASIC ACCESSORS **/

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
     * @brief Return the isMoving attribute.
     * @return The value of the isMoving attribute.
     */
    [[nodiscard]] bool getIsMoving() const;


    /** SPECIFIC ACCESSORS **/

    /**
     * @brief Get the bounding box of the platform.
     * @return SDL_Rect representing the platform box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;


    /** MODIFIERS **/

    /**
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state);


    /** PUBLIC METHODS **/

    /**
     * @brief Calculate the new position of the platform on x-axis and y-axis.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void applyMovement(double deltaTime);

private:
    /** ATTRIBUTES **/

    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    float speed; /**< The speed of the platform. */
    float moveX = 0; /**< The number of pixel the platform has moved on the x-axis*/
    float moveY = 0; /**< The number of pixel the platform has moved on the y-axis*/

    const Point left; /**< A Point representing the minimum position of the platform. */
    const Point right; /**< A Point representing the maximum position of the platform. */
    float ratio; /**< The ratio of pixel displacement between x-axis and y-axis. */
    bool start; /**< The place where the platform start : 0 for left/top, 1 for right/down */
    float directionX; /**< The current direction of the platform on x-axis : -1 for left/up, 1 for right/bottom */
    float directionY; /**< The current direction of the platform on y-axis : -1 for left/up, 1 for right/bottom */
    bool isMoving = true;

    float smoothingLimit = 3; /**< The smoothing limit of the platform movement */
};


#endif //PLAY_TOGETHER_MOVINGPLATFORM2D_H
