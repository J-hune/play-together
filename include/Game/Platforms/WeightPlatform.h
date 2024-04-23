#ifndef PLAY_TOGETHER_WEIGHTPLATFORM_H
#define PLAY_TOGETHER_WEIGHTPLATFORM_H

#include "IPlatform.h"

/**
 * @file WeightPlatform.h
 * @brief Defines the WeightPlatform class representing a weight platform.
 */

/**
 * @class WeightPlatform
 * @brief Represents a weight platform in a 2D game.
 */
class WeightPlatform : public IPlatform {
private:
    /* ATTRIBUTES */

    float startY; /**< The y-coordinate of the platform's starting position. */
    float x; /**< The x-coordinate of the platform's position. */
    float y = startY; /**< The y-coordinate of the platform's position. */
    float w; /**< The width of the platform. (in pixels) */
    float h; /**< The height of the platform. */
    float size; /**< The size of the platform. */
    float move = 0; /**< The number of pixel the platform has moved */
    float stepDistance; /**< The distance between each step of the platform. */
    float weight = 0; /**< The number of player on the platform. */
    float lerpSmoothingFactor = 11; /**< The smoothing factor for the lerp calculation. */
    bool isMoving = true; /** Flag indicating if the platform is currently moving. */
    bool isOnScreen = true; /**< Flag indicating if the platform is on the screen. */

    Texture texture; /**< The texture of the platform. */
    SDL_FRect textureOffsets; /**< The texture offsets of the platform adapted to the size. */

public:
    /* CONSTRUCTORS */

    WeightPlatform(float x, float y, float size, float stepDistance, const Texture& texture);

    bool operator==(const WeightPlatform &item) const {
        return x == item.getX()
               && y == item.getY()
               && w == item.getW()
               && h == item.getH();
    }


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

    /**
     * @brief Set the isOnScreen attribute.
     * @param state The new state of the isOnScreen attribute.
     */
    void setIsOnScreen(bool state) override;

    /**
     * @brief Increase the weight of the platform by 1.
     */
    void increaseWeight();

    /**
    * @brief Decrease the weight of the platform by 1.
    */
    void decreaseWeight();


    /* METHODS */

    /**
     * @brief Calculate the new position of the platform according to its weight.
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


#endif //PLAY_TOGETHER_WEIGHTPLATFORM_H
