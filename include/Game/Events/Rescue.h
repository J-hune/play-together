#ifndef PLAY_TOGETHER_RESCUE_H
#define PLAY_TOGETHER_RESCUE_H

#include "../Player.h"
#include "../../Physics/AABB.h"

/**
 * @file Rescue.h
 * @brief Defines the Rescue class representing a rescue zone.
 */

/**
 * @class Rescue
 * @brief Represents a rescue zone in a 2D game.
 */
class Rescue {
private :
    /* ATTRIBUTES */

    float x; /**< x coordinate of the rescue zone. */
    float y; /**< y coordinate of the rescue zone. */
    float w; /**< width of the rescue zone. */
    float h; /**< height of the rescue zone. */
    float currentSpawn = x; /**< current spawn point of the rescue zone. */
    float spawnStepDistance = 50; /**< distance between spawn points. */
    int id = -1; /**< id of the rescue zone. */


public :
    /* CONSTRUCTORS */

    explicit Rescue(float x, float y, float w, float h);
    Rescue() = default;


    /* ACCESSORS */

    /**
     * @brief Get the x coordinate of the rescue zone.
     * @return The x coordinate of the rescue zone.
     */
    [[nodiscard]] float getX() const;

    /**
     * @brief Get the y coordinate of the rescue zone.
     * @return The y coordinate of the rescue zone.
     */
    [[nodiscard]] float getY() const;

    /**
     * @brief Get the height of the rescue zone.
     * @return The height of the rescue zone.
     */
    [[nodiscard]] float getH() const;

    /**
     * @brief Get the width of the rescue zone.
     * @return The width of the rescue zone.
     */
    [[nodiscard]] float getW() const;

    /**
     * @brief Get the id of the rescue zone.
     * @return The id of the rescue zone.
     */
    [[nodiscard]] int getID() const;

    /**
     * @brief Get the next spawn position of the rescue zone.
     * @return The next spawn position of the rescue zone.
     */
    [[nodiscard]] float getNextPosition();

    /**
     * @brief Get the bounding box of the rescue zone.
     * @return The bounding box of the rescue zone.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;


    /* MODIFIERS */

    /**
     * @biref Set the rescue zone.
     * @param zone The new zone
     */
    void setZone(AABB zone);

};


#endif //PLAY_TOGETHER_RESCUE_H
