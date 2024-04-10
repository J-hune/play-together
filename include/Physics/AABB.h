#ifndef PLAY_TOGETHER_AABB_H
#define PLAY_TOGETHER_AABB_H

#include <SDL_rect.h>
#include "../Game/Point.h"

/**
 * @file AABB.h
 * @brief Defines the Axis-Aligned Bounding Box class responsible for handling AABBs in 2D space.
 */

enum class AABBType {
    SAVE,
    TOGGLE_GRAVITY,
    INCREASE_FALL_SPEED,
};

class AABB {
public:

    /** CONSTRUCTOR **/

    AABB(float x, float y, float width, float height, AABBType type);


    /** ACCESSORS **/

    /**
     * @brief Get the x-coordinate of the AABB.
     * @return The x-coordinate of the AABB.
     */
    [[nodiscard]] float getX() const;

    /**
     * @brief Get the y-coordinate of the AABB.
     * @return The y-coordinate of the AABB.
     */
    [[nodiscard]] float getY() const;

    /**
     * @brief Get the width of the AABB.
     * @return The width of the AABB.
     */
    [[nodiscard]] float getWidth() const;

    /**
     * @brief Get the height of the AABB.
     * @return The height of the AABB.
     */
    [[nodiscard]] float getHeight() const;

    /**
     * @brief Get the type of the AABB.
     * @return The type of the AABB.
     */
    [[nodiscard]] AABBType getType() const;


    [[nodiscard]] SDL_FRect getRect() const;


    /** MODIFIERS **/

    /**
     * @brief Set the x-coordinate of the AABB.
     * @param value The x-coordinate of the AABB.
     */
    void setX(float value);

    /**
     * @brief Set the y-coordinate of the AABB.
     * @param value The y-coordinate of the AABB.
     */
    void setY(float value);

    /**
     * @brief Set the width of the AABB.
     * @param value The width of the AABB.
     */
    void setWidth(float value);

    /**
     * @brief Set the height of the AABB.
     * @param height The height of the AABB.
     */
    void setHeight(float height);

    /**
     * @brief Set the type of the AABB.
     * @param value The type of the AABB.
     */
    void setType(AABBType value);

    /**
     * @brief Set the position of the AABB.
     * @param position The position of the AABB.
     */
    void setPosition(const Point &position);

private:
    /** ATTRIBUTES **/

    float x; /**< The x-coordinate of the AABB. */
    float y; /**< The y-coordinate of the AABB. */
    float width; /**< The width of the AABB. */
    float height; /**< The height of the AABB. */
    AABBType type; /**< The type of the AABB. */
};


#endif //PLAY_TOGETHER_AABB_H
