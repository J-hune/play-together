#ifndef PLAY_TOGETHER_SPRITE_H
#define PLAY_TOGETHER_SPRITE_H

#include <SDL.h>
#include <string>
#include "Animation.h"

/**
 * @file Sprite.h
 * @brief Defines the Sprite class representing a Sprite in a 2D game.
 */

/**
 * @class Sprite
 * @brief Represents a sprite in a 2D game with its animation and texture.
 */
class Sprite {
public:
    /** CONSTRUCTORS **/

    Sprite() = default; // Used for the useless empty Game constructor of Athena

    /**
     * @brief Constructor for the Sprite class.
     * @param animation Initial animation of the sprite.
     * @param texture The texture used for the sprite.
     * @param width The width of a tile in the texture.
     * @param height he height of a tile in the texture.
     */
    Sprite(Animation animation, SDL_Texture **texture, int width, int height);


    /** BASIC ACCESSORS **/

    /**
     * @brief Return the texture attribute.
     * @return A SDL_Texture representing the sprite texture.
     */
    [[nodiscard]] SDL_Texture* getTexture() const;

    /**
     * @brief Return the srcRect attribute.
     * @return A SDL_Rect representing the srcRect attribute.
     */
    [[nodiscard]] SDL_Rect getSrcRect() const;


    /** SPECIFIC ACCESSORS **/

    /**
     * @brief Return the combination of flipVertical and flipHorizontal attributes.
     * @return A SDL_RenderFlip representing flip value for the renderer.
     */
    [[nodiscard]] SDL_RendererFlip getFlip() const;


    /** MODIFIERS **/

    /**
     * @brief Set the animation attribute.
     * @param animation The new Animation of sprite.
     */
    void setAnimation(Animation newAnimation);

    /**
     * @brief Set the flipHorizontal attribute.
     * @param flip The new flip value of the flipHorizontal attribute.
     */
    void setFlipHorizontal(SDL_RendererFlip flip);


    /**
     * @brief Set the flipVertical attribute.
     * @param newFlip The new flip value of the flipVertical attribute.
     * @see toggleFlipVertical() for a similar operation.
     */
    void setFlipVertical(SDL_RendererFlip flip);

    /**
     * @brief Toggle the flipVertical attribute (SDL_FLIP_VERTICAL or SDL_FLIP_NONE).
     * @see setFlipVertical() for a similar operation.
     */
    void toggleFlipVertical();


    /** PUBLIC METHODS **/

    /**
     * @brief Update the sprite animation.
     */
    void updateAnimation();


private:
    /** ATTRIBUTES **/

    Animation animation; /**< The current animation of the sprite. */
    SDL_Texture *texture; /**< The texture of the sprite. */
    SDL_Rect srcRect; /**< The square that will be copied in the texture. */
    SDL_RendererFlip flipHorizontal = SDL_FLIP_NONE; /**< If the sprite is flipped horizontally. */
    SDL_RendererFlip flipVertical = SDL_FLIP_NONE; /**< If the sprite is flipped vertically. */

};


#endif //PLAY_TOGETHER_SPRITE_H
