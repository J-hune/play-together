#ifndef PLAY_TOGETHER_TEXTURE_H
#define PLAY_TOGETHER_TEXTURE_H

#include <SDL.h>
#include "../Game/Point.h"


constexpr char TEXTURES_DIRECTORY[] = "assets/textures/";
constexpr char SPRITES_DIRECTORY[] = "assets/sprites/";

/**
 * @file Texture.h
 * @brief Defines the Texture class representing a texture in a 2D game.
 */

/**
 * @class Texture
 * @brief Represents a Texture in a 2D game.
 */
class Texture {
private:
    /* ATTRIBUTES */

    SDL_Texture *texturePtr = nullptr; /**< The texture of the sprite. */
    SDL_Rect sizeRect = {0, 0, 0, 0}; /**< The square representing the size of the texture. */
    SDL_FRect offsets; /**< The offset of the texture compared to the collision box. */
    SDL_RendererFlip flipHorizontal = SDL_FLIP_NONE; /**< If the sprite is flipped horizontally. */
    SDL_RendererFlip flipVertical = SDL_FLIP_NONE; /**< If the sprite is flipped vertically. */

public:
    /* CONSTRUCTORS */

    Texture() = default;
    explicit Texture(SDL_Texture &texture, SDL_FRect offsets = {0, 0, 0, 0});


    /* ACCESSORS */

    /**
     * @brief Return the texture attribute.
     * @return A SDL_Texture representing the sprite texture.
     */
    [[nodiscard]] SDL_Texture* getTexture() const;

    /**
     * @brief Return the size attribute.
     * @return A SDL_Rect representing the size of the texture.
     */
    [[nodiscard]] SDL_Rect getSize() const;

    /**
     * @brief Return the offsets attribute.
     * @return A SDL_FRect representing the offsets of the texture.
     */
    [[nodiscard]] SDL_FRect getOffsets() const;

    /**
     * @brief Return the flipHorizontal attribute.
     * @return A SDL_RendererFlip representing flip value for the renderer.
     */
    [[nodiscard]] SDL_RendererFlip getFlipHorizontal() const;

    /**
     * @brief Return the combination of flipVertical and flipHorizontal attributes.
     * @return A SDL_RenderFlip representing flip value for the renderer.
     */
    [[nodiscard]] SDL_RendererFlip getFlip() const;


    /* MODIFIERS */

    /**
     * @brief Set the texture attribute.
     * @param texture The new texture of the sprite.
     */
    void setTexture(SDL_Texture &newTexture);

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

};


#endif //PLAY_TOGETHER_TEXTURE_H
