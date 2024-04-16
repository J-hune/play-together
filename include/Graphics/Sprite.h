#ifndef PLAY_TOGETHER_SPRITE_H
#define PLAY_TOGETHER_SPRITE_H

#include "Texture.h"
#include "Animation.h"


/**
 * @file Sprite.h
 * @brief Defines the Sprite class representing a Sprite in a 2D game.
 */

/**
 * @class Sprite
 * @brief Represents a sprite in a 2D game with its animation and texture.
 */
class Sprite : public Texture {
private:
    /* ATTRIBUTES */

    // Animation attributes
    Animation animation = {}; /**< The current animation of the sprite. */
    int animationIndexX = 0; /**< The current position in the animation of the sprite. */
    Uint32 lastAnimationUpdate = SDL_GetTicks(); /**< The last time the animation was updated. */
    SDL_Rect srcRect = {0 , 0, 0, 0}; /**< The square that will be copied in the texture. */

    // Unique animation attributes
    Animation nextAnimation = animation; /**< The animation that will play after an animation of 'unique' type. */
    int nbFrameDisplayed = 0; /**< The number of frame that has passed since the current animation was display. */
    bool uniqueAnimationIsDisplayed = false; /**< Flag indicating if a unique animation is currently displayed. */


public:
    /* CONSTRUCTORS */

    Sprite() = default;

    /**
     * @brief Constructor for the Sprite class.
     * @param animation Initial animation of the sprite.
     * @param texture The texture used for the sprite.
     * @param width The width of a tile in the texture.
     * @param height he height of a tile in the texture.
     */
    Sprite(SDL_Texture &texture, Animation animation, int width, int height);


    /* ACCESSORS */

    /**
     * @brief Return the animation attribute.
     * @return An Animation representing the animation.
     */
    [[nodiscard]] Animation getAnimation() const;

    /**
     * @brief Return the srcRect attribute.
     * @return A SDL_Rect representing the srcRect attribute.
     */
    [[nodiscard]] SDL_Rect getSrcRect() const;


    /* MODIFIERS */

    /**
     * @brief Set the animation attribute.
     * @param animation The new Animation of the sprite.
     */
    void setAnimation(const Animation& newAnimation);

    /**
     * @brief Set the animationIndexX attribute.
     * @param texture The new index of the animationIndexX.
     */
    void setAnimationIndexX(int index);


    /* PUBLIC METHODS */

    /**
     * @brief Update the sprite animation.
     */
    void updateAnimation();

};


#endif //PLAY_TOGETHER_SPRITE_H
