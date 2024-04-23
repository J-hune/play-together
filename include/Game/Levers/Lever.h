#ifndef PLAY_TOGETHER_LEVER_H
#define PLAY_TOGETHER_LEVER_H

#include <SDL_rect.h>
#include "../../Graphics/Texture.h"
#include "../../Sounds/SoundEffect.h"

/**
 * @file Lever.h
 * @brief Defines the Lever class representing a lever.
 */

/**
 * @class Lever
 * @brief Represents a lever in a 2D game.
 */
class Lever {
private:
    /* ATTRIBUTES */

    float x; /**< The x-coordinate of the lever's position. */
    float y; /**< The y-coordinate of the lever's position. */
    float w; /**< The width of the lever. */
    float h; /**< The height of the lever. */
    float size; /**< The size of the lever. */
    bool isActivated; /**< The state of the lever. */
    bool isOnScreen = true; /**< Flag indicating if the lever is on screen. */

    Texture texture; /**< The texture of the lever. */
    SDL_FRect textureOffsets; /**< The texture offsets of the lever adapted to the size. */
    SoundEffect activationSound = SoundEffect("lever.wav"); /**< The sound effect associated to lever's activation. */

public:

    /* CONSTRUCTORS */

    Lever(float x, float y, float size, bool isActivated, const Texture& texture);
    virtual ~Lever() = default;


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
     * @brief Return the state of the lever.
     * @return The state of the lever.
     */
    [[nodiscard]] bool getIsActivated() const;

    /**
     * @brief Return the bounding box of the lever.
     * @return The bounding box of the lever.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;


    /* MUTATORS */

    /**
     * @brief Set the state of the lever.
     * @param isActivated The new state of the lever.
     */
    void setIsActivated(bool isActivated);

    /**
     * @brief Set the isOnScreen attribute.
     * @param state The new state of the isOnScreen attribute.
     */
    void setIsOnScreen(bool isOnScreen);

    /**
     * @brief Toggle the state of the lever.
     */
    void toggleIsActivated();


    /* METHODS */

    /**
     * @brief Overloaded equality operator.
     * @param lever The lever object to compare with.
     * @return True if the levers are equal, false otherwise.
     */
    bool operator==(const Lever& lever) const;

    /**
     * @brief Render the lever by drawing its texture.
     * @param renderer The renderer used to render the lever.
     * @param camera The camera used to render the lever.
     */
    void render(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the lver by its drawing its collision box.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;


private:

    /**
     * @brief Apply the effect of the lever.
     */
    virtual void applyEffect() const = 0;
};


#endif //PLAY_TOGETHER_LEVER_H
