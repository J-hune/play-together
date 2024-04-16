#ifndef PLAY_TOGETHER_SWITCHINGPLATFORM_H
#define PLAY_TOGETHER_SWITCHINGPLATFORM_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>
#include <vector>
#include "IPlatform.h"

/**
 * @file SwitchingPlatform.h
 * @brief Defines the SwitchingPlatform class representing a switching platform.
 */

/**
 * @class SwitchingPlatform
 * @brief Represents a switching platform in a 2D game.
 */
class SwitchingPlatform : public IPlatform {
private:
    /* ATTRIBUTES */

    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    Texture texture; /**< The texture of the platform. */
    double bpm; /**< The beat per minute of the platform. */
    Uint32 startTime = SDL_GetTicks(); /**< The time set at the beginning of every beat */
    int actualPoint = 0; /**< The current point the platform's position. */
    std::vector<Point> steps; /** Collection of Point representing every possible position of the platform. */
    bool isMoving = true; /** Flag indicating if the platform is currently moving. */


public:
    /* CONSTRUCTORS */

    SwitchingPlatform(float x, float y, float w, float h, const Texture& texture, Uint32 bpm, std::vector<Point> steps);


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
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state) override;


    /* PUBLIC METHODS */

    /**
     * @brief Calculate the new position of the platform according to its bpm.
     */
    void applyMovement([[maybe_unused]] double delta_time) override;

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


#endif //PLAY_TOGETHER_SWITCHINGPLATFORM_H
