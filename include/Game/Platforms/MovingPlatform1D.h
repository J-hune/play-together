#ifndef PLAY_TOGETHER_MOVINGPLATFORM1D_H
#define PLAY_TOGETHER_MOVINGPLATFORM1D_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>
#include "IPlatform.h"

constexpr float PLATFORM_LERP_SMOOTHING_FACTOR = 0.05F;

/**
 * @file MovingPlatform1D.h
 * @brief Defines the MovingPlatform1D class representing a MovingPlatform on x-axis.
 */

/**
 * @class MovingPlatform1D
 * @brief Represents a moving platform on an axis in a 2D game.
 */
class MovingPlatform1D : public IPlatform {
private:
    /* ATTRIBUTES */

    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    float size; /**< The size of the platform. */
    float speed; /**< The speed of the platform. */
    float move = 0; /**< The number of pixel the platform has moved */
    PlatformBuffer buffer = {0, 0}; /**< The buffer of the player */

    const float min; /**< The minimum x-coordinate of the platform's position. */
    const float max; /**< The maximum x-coordinate of the platform's position. */
    bool start; /**< The place where the platform start : 0 for left/top, 1 for right/down */
    float direction; /**< The current direction of the platform : -1 for left/up, 1 for right/bottom */
    bool axis; /**< The axis on which the platform moves : 0 for x-axis, 1 for y-axis */
    bool isMoving = true; /** Flag indicating if the platform is currently moving. */
    float smoothingLimit = 2; /**< The smoothing limit of the platform movement */

    Texture texture; /**< The texture of the platform. */
    SDL_FRect textureOffsets; /**< The texture offsets of the platform adapted to the size. */

public:
    /* CONSTRUCTORS */

    MovingPlatform1D(float x, float y, float size, float speed, float min, float max, bool start, bool axis, const Texture& texture);


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
     * @brief Return the move attribute.
     * @return The value of the move attribute.
     */
    [[nodiscard]] float getMove() const;

    /**
     * @brief Return the axis attribute.
     * @return The value of the axis attribute.
     */
    [[nodiscard]] bool getAxis() const;

    /**
     * @brief Return the direction attribute.
     * @return The value of the direction attribute.
     */
    [[nodiscard]] float getDirection() const;

    /**
     * @brief Return the isMoving attribute.
     * @return The value of the isMoving attribute.
     */
    [[nodiscard]] bool getIsMoving() const override;

    /**
     * @brief Get the bounding box of the platform.
     * @return SDL_Rect representing the platform box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const override;


    /* MODIFIERS */

    /**
     * @brief Set the x attribute.
     * @param value The new x-coordinate of the platform's position.
     */
    void setX(float value);

    /**
     * @brief Set the y attribute.
     * @param value The new y-coordinate of the platform's position.
     */
    void setY(float value);

    /**
     * @brief Set the move attribute.
     * @param value The new move of the platform.
     */
    void setMove(float value);

    /**
     * @brief Set the direction attribute.
     * @param value The new direction of the platform.
     */
    void setDirection(float value);

    /**
    * @brief Sets the buffer attribute.
    * @param value The new value of the buffer attribute.
    */
    void setBuffer(PlatformBuffer value);

    /**
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state) override;


    /* PUBLIC METHODS */

    /**
     * @brief Selects whether to calculate x- or y-axis movement.
     * @param delta_time The time elapsed since the last frame in seconds.
     * @see applyXaxisMovement() and applyYaxisMovement() for sub-functions.
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


private:

    /**
     * @brief Calculate the new position of the platform on x-axis.
     * @param delta_time The time elapsed since the last frame in seconds.
     */
    void applyXaxisMovement(double delta_time);

    /**
     * @brief Calculate the new position of the platform on y-axis.
     * @param delta_time The time elapsed since the last frame in seconds.
     */
    void applyYaxisMovement(double delta_time);

};


#endif //PLAY_TOGETHER_MOVINGPLATFORM1D_H
