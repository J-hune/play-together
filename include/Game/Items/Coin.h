#ifndef PLAY_TOGETHER_COIN_H
#define PLAY_TOGETHER_COIN_H

#include "Item.h"

/**
 * @file Coin.h
 * @brief Defines the Coin class responsible for Coin object.
 */
class Coin : public Item {
private:
    /* ATTRIBUTES */

    int value; /**< The score to add to the player. */
    Sprite *spritePtr;
    static Sprite sprite; /**< The sprite of the item. */

    // LOADED TEXTURE
    static SDL_Texture *spriteTexturePtr; /**< The base texture of a item */

    // SPRITE ANIMATIONS
    static constexpr Animation gold = {0, 8, 100, false}; /**< Gold animation */
    static constexpr Animation silver = {1, 8, 100, false}; /**< Silver animation */
    static constexpr Animation bronze = {2, 8, 100, false}; /**< Bronze animation */

public:
    /* CONSTRUCTORS */

    Coin(float X, float Y, float width, float height, int score);
    ~Coin() override = default;


    /* ACCESSORS */

    /**
     * @brief Return the value attribute.
     * @return The value of the value attribute.
     */
    [[nodiscard]] int getValue() const;


    /* MUTATORS */

    /**
     * @brief Sets the value attribute.
     * @param newValue The new value of the value attribute.
     */
    void setValue(int newValue);


    /* METHODS */

    /**
     * @brief Load the coin texture.
     * @param renderer The renderer of the game.
     * @return Returns true if the texture was loaded correctly, false otherwise.
     */
    static bool loadTexture(SDL_Renderer &renderer);

    /**
     * @brief Apply the item's effect to a player.
     * @param player The player the effect will be applied to.
     */
    void applyEffect(Player &player) override;

    /**
     * @brief Renders the coin's sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera) override;
};


#endif //PLAY_TOGETHER_COIN_H
