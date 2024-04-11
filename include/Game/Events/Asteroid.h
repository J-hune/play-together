#ifndef PLAY_TOGETHER_ASTEROID_H
#define PLAY_TOGETHER_ASTEROID_H


#include "../../Graphics/Sprite.h"
#include "../Point.h"
#include "../Camera.h"
#include "../../Sounds/SoundEffect.h"
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <random>

class Asteroid {
public :
    /** CONSTRUCTOR **/

    /**
     * @brief Constructor for the Asteroid class.
     * @param x The x position of the interval start.
     * @param y The y position of the interval start.
     */
    Asteroid(float x, float y, size_t seed);

    /**
     * @brief Constructor for the Asteroid class.
     * @param x The x position of the asteroid.
     * @param y The y position of the asteroid.
     * @param h Height of the asteroid.
     * @param w Width of the asteroid.
     * @param angle Angle of the player.
     */
    Asteroid(float x, float y, float speed, float h, float w, float angle);


    /** BASIC ACCESSORS **/

    /**
     * @brief Return the x attribute.
     * @return The value of the x attribute
     */
    [[nodiscard]] float getX() const;

    /**
     * @brief Return the y attribute.
     * @return The value of the y attribute
     */
    [[nodiscard]] float getY() const;

    /**
     * @brief Return the speed attribute.
     * @return The value of the speed attribute
     */
    [[nodiscard]] float getSpeed() const;

    /**
     * @brief Return the weight attribute.
     * @return The value of the weight attribute
     */
    [[nodiscard]] float getW() const;

    /**
     * @brief Return the height attribute.
     * @return The value of the height attribute
     */
    [[nodiscard]] float getH() const;

    /**
     * @brief Return the angle attribute.
     * @return The value of the angle attribute
     */
    [[nodiscard]] float getAngle() const;


    /**
     * @brief Return the sprite attribute.
     * @return A pointer of a sprite object representing the sprite of the asteroid.
     */
    [[nodiscard]] Sprite *getSprite();


    /** SPECIFIC ACCESSORS **/

    /**
     * @brief Get the bounding box of the asteroid.
     * @return SDL_Rect representing the bounding box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;

    /**
     * @brief Get the vertices of the player's bounding box.
     * @return A vector of Point representing the vertices.
     */
    [[nodiscard]] std::vector<Point> getVertices() const;


    /** MODIFIERS **/

    /**
     * @brief Set the x attribute.
     * @param val The new value of the x attribute.
     */
    void setX(float val);

    /**
     * @brief Set the y attribute.
     * @param val The new value of the y attribute.
     */
    void setY(float val);


    /**
     * @brief Set the width attribute.
     * @param val The new value of the width attribute.
     */
    void setW(float val);

    /**
     * @brief Set the height attribute.
     * @param val The new value of the height attribute.
     */
    void setH(float val);

    /**
     * @brief Set the angle attribute.
     * @param val The new value of the angle attribute.
     */
    void setAngle(float val);


    /** PUBLIC METHODS **/

    /**
     * @brief Load all asteroid textures.
     * @param renderer The renderer of the game.
     * @return Returns true if all textures were loaded correctly, false otherwise.
     */
    static bool loadTextures(SDL_Renderer &renderer);

    /**
     * @brief Renders the asteroid's sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera);

    /**
     * @brief Renders the asteroid's collision box.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Applies the movement of the asteroid based on its speed and angle.
     *        Updates the current position coordinates accordingly.
     * TODO: correct the movement according to the delta time
     */
    void applyMovement(double delta_time);


    /**
     * @brief Generates an array of possible positions for the asteroid.
     * @param position_count The number of positions to generate.
     * @param x The lower bound of the position range.
     * @param y The upper bound of the position range.
     * @param seed The seed for random number generation.
     */
    static void generateRandomPositionsArray(int position_count, float x, float y, size_t seed);

    /**
     * @brief Get a random position from the array of possible positions.
     * @param seed The seed for random number generation. If zero, a random seed is generated.
     * @return A randomly selected position.
     */

    static float getRandomPosition(size_t seed);

    /**
     * @brief Get an array of possible angles for the asteroid.
     * @param angle_count The number of angles to generate.
     * @param seed The seed for random number generation.
     */
    static void generateRandomAnglesArray(int angle_count, size_t seed);

    /**
    * @brief Generates a random angle from the array of possible angles.
    * @param seed The seed for random number generation. If zero, a random seed is generated.
    * @return A randomly selected angle.
    */
    static float getRandomAngle(size_t seed);

    /**
      * @brief Triggers the explosion effect for the asteroid.
      *        This typically involves changing the sprite's animation to an explosion animation.
      */
    void explode();


private :
    float x = 0; /**< The x-coordinate of the asteroid's position. */
    float y = 0; /**< The y-coordinate of the asteroid's position. */
    float h = 80; /**< The height of the asteroid. */
    float w = 80; /**< The width of the asteroid.*/

    float speed = 0; /**< The speed of the asteroid. */
    float horizontalSpeed = 0;/**< The horizontal speed of the asteroid. */
    float verticalSpeed = 0; /**< The vertical speed of the asteroid. */

    float angle = 0; /**< The angle of the asteroid. */
    float angle_radians = 0; /**< The angle radians of the asteroid. */

    Sprite sprite; /**< The sprite of the player. */
    SoundEffect explosionSound; /**< The sound effect associated to asteroid's explosion. */

    static std::vector<float> positions; /** Possible position array for the asteroid.*/
    static std::vector<int> positionsLock; /** Array of locked asteroid positions. */
    static std::vector<float> angles; /** Possible angle array for the asteroid.*/
    static std::vector<int> anglesLock; /** Array of locked asteroid angles. */

    // LOADED TEXTURE
    static SDL_Texture *spriteTexturePtr; /**< The texture of asteroid. */

    // SPRITE ANIMATIONS
    static constexpr Animation idle = {0, 8, 100, false}; /**< Idle animation */
    static constexpr Animation explosion = {6, 6, 70, true}; /**< Explosion animation */

};


#endif //PLAY_TOGETHER_ASTEROID_H

