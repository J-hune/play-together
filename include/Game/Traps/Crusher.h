#ifndef PLAY_TOGETHER_CRUSHER_H
#define PLAY_TOGETHER_CRUSHER_H

#include "../../Graphics/Texture.h"
#include "../Point.h"
#include "../../Sounds/SoundEffect.h"

struct CrusherBuffer {
    float deltaX;
    float deltaY;
};

class Crusher {
private:

    // CHARACTERISTICS ATTRIBUTES
    float x; /**< The x-coordinate of the crusher's position. */
    float y; /**< The y-coordinate of the crusher's position. */
    float w; /**< The width of the crusher. (in pixels) */
    float h; /**< The height of the crusher. */
    float size; /**< The size of the crusher. */
    const float min; /**< The minimum x-coordinate of the crusher's position. */
    const float max; /**< The maximum x-coordinate of the crusher's position. */
    const float pixelToMove = 100; /**< The number of pixels the crusher moves per second. */
    float direction = 1; /**< The direction of the crusher's movement. */
    bool isCrushing = false; /**< Flag indicating if the crusher is currently crushing. */
    bool isMoving = true; /** Flag indicating if the crusher is currently moving. */
    bool isOnScreen = true; /**< Flag indicating if the crusher is currently on screen. */
    CrusherBuffer buffer = {0, 0}; /**< The buffer of the crusher */

    // TIME ATTRIBUTES
    int timer = 0; /**< The timer of the crusher. */
    Uint32 lastUpdate = SDL_GetTicks(); /**< The last time the crusher was updated. */
    const Uint32 moveUpTime; /**< The time the crusher will take to move up in seconds. */
    const Uint32 waitUpTime; /**< The time to wait up before going down in milliseconds. */
    const Uint32 waitDownTime; /**< The time to wait down before going up in milliseconds. */

    // RENDERING ATTRIBUTES
    Texture texture; /**< The texture of the crusher. */
    SDL_FRect textureOffsets; /**< The texture offsets of the crusher adapted to the size. */
    SoundEffect crushingSound = SoundEffect("Traps/crushing.wav"); /**< The sound effect associated to the crusher. */


public:
    /* CONSTRUCTORS */

    Crusher(float x, float y, float size, float min, float max, Uint32 moveUpTime, Uint32 waitUpTime, Uint32 waitDownTime, const Texture& texture);


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
     * @brief Return the direction attribute.
     * @return The value of the direction attribute.
     */
    [[nodiscard]] float getDirection() const;

    /**
     * @brief Return the isCrushing attribute.
     * @return The value of the isCrushing attribute.
     */
    [[nodiscard]] bool getIsCrushing() const;

    /**
     * @brief Return the isMoving attribute.
     * @return The value of the isMoving attribute.
     */
    [[nodiscard]] bool getIsMoving() const;

    /**
     * @brief Return the isOnScreen attribute.
     * @return The value of the isOnScreen attribute.
     */
    [[nodiscard]] bool getIsOnScreen() const;

    /**
     * @brief Get the bounding box of the crusher.
     * @return SDL_Rect representing the crusher box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;

    /**
     * @brief Get the bounding box of the crusher's crushing zone.
     * @return SDL_Rect representing the crusher's crushing zone box.
     */
    [[nodiscard]] SDL_FRect getCrushingZoneBoundingBox() const;


    /* MODIFIERS */

    /**
     * @brief Set the x attribute.
     * @param value The new value of the x attribute.
     */
    void setX(float value);

    /**
     * @brief Set the y attribute.
     * @param value The new value of the y attribute.
     */
    void setY(float value);

    /**
     * @brief Set the direction attribute.
     * @param value The new value of the direction attribute.
     */
    void setDirection(float value);

    /**
     * @brief Set the buffer attribute.
     * @param value The new value of the buffer attribute.
     */
    void setBuffer(CrusherBuffer value);

    /**
     * @brief Set the isMoving attribute.
     * @param state The new state of the isMoving attribute.
     */
    void setIsMoving(bool state);

    /**
     * @brief Set the isOnScreen attribute.
     * @param state The new state of the isOnScreen attribute.
     */
    void setIsOnScreen(bool state);


    /* METHODS */

    /**
     * @brief Calculate the new position of the crusher according.
     * @param delta_time Represents the time elapsed since the last update.
     * @return Returns true if reach the end of his down movement to make camera shake, false otherwise.
     * @see applyUpMovement() and applyDownMovement() for sub-functions.
     */
    bool applyMovement(double delta_time);

    /**
     * @brief Renders the crusher by drawing its textures.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the crusher by its drawing its collision box.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;


private:

    /**
     * @brief Apply the movement of the crusher when it is going up.
     * @param delta_time Represents the time elapsed since the last update.
     * @see applyMovement() for main use.
     */
    void applyUpMovement(double delta_time);

    /**
     * @brief Apply the movement of the crusher when it is going down.
     * @param delta_time Represents the time elapsed since the last update.
     * @return Returns true if reach the end of his down movement to make camera shake, false otherwise.
     * @see applyMovement() for main use.
     */
    bool applyDownMovement(double delta_time);
};


#endif //PLAY_TOGETHER_CRUSHER_H
