#ifndef PLAY_TOGETHER_MOVINGPLATFORM2D_H
#define PLAY_TOGETHER_MOVINGPLATFORM2D_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>
#include "IPlatform.h"

/**
 * @file MovingPlatform2D.h
 * @brief Defines the MovingPlatform2D class representing a moving platform on x-axis and y-axis.
 */

/**
 * @class MovingPlatform2D
 * @brief Represents a moving platform on two axis in a 2D game.
 */
class MovingPlatform2D : public IPlatform {
private:
    /* ATTRIBUTES */

    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    Texture texture; /**< The texture of the platform. */
    float speed; /**< The speed of the platform. */
    float moveX = 0; /**< The number of pixel the platform has moved on the x-axis*/
    float moveY = 0; /**< The number of pixel the platform has moved on the y-axis*/

    const Point left; /**< A Point representing the minimum position of the platform. */
    const Point right; /**< A Point representing the maximum position of the platform. */
    float ratio; /**< The ratio of pixel displacement between x-axis and y-axis. */
    bool start; /**< The place where the platform start : 0 for left/top, 1 for right/down */
    float directionX; /**< The current direction of the platform on x-axis : -1 for left/up, 1 for right/bottom */
    float directionY; /**< The current direction of the platform on y-axis : -1 for left/up, 1 for right/bottom */
    bool isMoving = true; /** Flag indicating if the platform is currently moving. */

    float smoothingLimit = 3; /**< The smoothing limit of the platform movement */


public:
    /* CONSTRUCTORS */

    MovingPlatform2D(float x, float y, float w, float h, const Texture& texture,float speed, Point left, Point right, bool start);


    /* ACCESSORS */

    /**
     * @brief Return the x attribute.
     * @return The value of the x attribute.
     */
    [[nodiscard]] float getX() const override;

    /**
     * @brief Return the y attribute.
     * @return The value of the y attribute.
     */
    [[nodiscard]] float getY() const override;

    /**
     * @brief Return the weight attribute.
     * @return The value of the weight attribute.
     */
    [[nodiscard]] float getW() const override;

    /**
     * @brief Return the height attribute.
     * @return The value of the height attribute.
     */
    [[nodiscard]] float getH() const override;

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
    [[nodiscard]] bool getIsMoving() const override;


    /* SPECIFIC ACCESSORS */

    /**
     * @brief Get the bounding box of the platform.
     * @return SDL_Rect representing the platform box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const override;


    /* MODIFIERS */

    /**
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state) override;


    /* PUBLIC METHODS */

    /**
     * @brief Calculate the new position of the platform on x-axis and y-axis.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void applyMovement(double delta_time) override;

    /**
     * @brief Renders the platforms by drawing its textures.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera) const override;

    /**
     * @brief Renders the platforms by its drawing its collision box.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const override;

};


#endif //PLAY_TOGETHER_MOVINGPLATFORM2D_H
