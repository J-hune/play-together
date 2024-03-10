#ifndef PLAY_TOGETHER_MOVINGPLATFORM1D_H
#define PLAY_TOGETHER_MOVINGPLATFORM1D_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>

constexpr float PLATFORM_LERP_SMOOTHING_FACTOR = 0.05F;

/**
 * @file MovingPlatform1D.h
 * @brief Defines the MovingPlatform1D class representing a MovingPlatform on x-axis.
 */

/**
 * @class MovingPlatform1D
 * @brief Represents a moving platform on an axis in a 2D game.
 */

class MovingPlatform1D {
public:
    /** CONSTRUCTOR **/

    MovingPlatform1D(float x, float y, float w, float h, float speed, float minX, float maxX, bool start, bool axis);


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
     * @brief Return the move attribute.
     * @return The value of the move attribute.
     */
    [[nodiscard]] float getMove() const;

    /**
     * @brief Return the axis attribute.
     * @return The value of the axis attribute.
     */
    [[nodiscard]] bool getAxis() const;


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
     * @brief Selects whether to calculate x- or y-axis movement.
     * @see applyXaxisMovement() and applyYaxisMovement() for applying movement.
     */
    void applyMovement();

private:
    /** ATTRIBUTES **/

    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    float speed; /**< The speed of the platform. */
    float move = 0; /**< The number of pixel the platform has moved */

    const float min; /**< The minimum x-coordinate of the platform's position. */
    const float max; /**< The maximum x-coordinate of the platform's position. */
    bool start; /**< The place where the platform start : 0 for left/top, 1 for right/down */
    float direction; /**< The current direction of the platform : -1 for left/up, 1 for right/bottom */
    bool axis; /**< The axis on which the platform moves : 0 for x-axis, 1 for y-axis */
    bool isMoving = true;

    float smoothingLimit = 2; /**< The smoothing limit of the platform movement */


    /** PRIVATE METHODS **/

    /**
     * @brief Calculate the new position of the platform on x-axis.
     */
    void applyXaxisMovement();

    /**
     * @brief Calculate the new position of the platform on y-axis.
     */
    void applyYaxisMovement();

};


#endif //PLAY_TOGETHER_MOVINGPLATFORM1D_H
