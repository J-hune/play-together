#ifndef PLAY_TOGETHER_SWITCHINGPLATFORM_H
#define PLAY_TOGETHER_SWITCHINGPLATFORM_H

#include <SDL.h>
#include <cstdlib>
#include <ranges>
#include <vector>
#include "../Point.h"

/**
 * @file SwitchingPlatform.h
 * @brief Defines the SwitchingPlatform class representing a switching platform.
 */

/**
 * @class SwitchingPlatform
 * @brief Represents a switching platform in a 2D game.
 */

class SwitchingPlatform {
public:
    /** CONSTRUCTOR **/

    SwitchingPlatform(float x, float y, float w, float h, Uint32 bpm, std::vector<Point> points);


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
     * @brief Calculate the new position of the platform according to its bpm.
     */
    void applyMovement();

private:
    float x; /**< The x-coordinate of the platform's position. */
    float y; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    double bpm; /**< The beat per minute of the platform. */
    Uint32 startTime; /**< The time set at the beginning of every beat */
    int actualPoint = 0; /**< The current point the platform's position. */
    std::vector<Point> points; /** Collection of Point representing every possible position of the platform. */
    bool isMoving = true;

};


#endif //PLAY_TOGETHER_SWITCHINGPLATFORM_H
