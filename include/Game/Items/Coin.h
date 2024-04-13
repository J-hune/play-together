#ifndef PLAY_TOGETHER_COIN_H
#define PLAY_TOGETHER_COIN_H

#include "Item.h"

/**
 * @file Coin.h
 * @brief Defines the SizePowerUp class responsible for SizePowerUp object.
 */
class Coin : public Item {
private:
    static constexpr Animation gold = {0, 8, 100}; /**< Gold animation */
    static constexpr Animation silver = {1, 8, 100}; /**< Silver animation */
    static constexpr Animation bronze = {2, 8, 100}; /**< Bronze animation */

    Sprite *spritePtr;

    static Sprite sprite; /**< The sprite of the item. */
    static SDL_Texture *baseSpriteTexturePtr; /**< The base texture of a item */

    /* ATTRIBUTES */

    int value; /**< The score to add to the player. */

public:

    /**
     * @brief Return the value attribute.
     * @return The value of the value attribute.
     */
    [[nodiscard]] int getValue() const;

    /**
     * @brief Sets the value attribute.
     * @param newValue The new value of the value attribute.
     */
    void setValue(int newValue);

    /* CONSTRUCTORS */
    Coin(float X, float Y, float width, float height, int score);
    ~Coin() override = default;


    /* METHODS */

    static void setAnimation(Animation newAnimation);

    static bool loadTextures(SDL_Renderer &renderer);

    static void updateSprite();


    /**
     * @brief Renders the player's sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, Point camera) const;


    /**
     * @brief Apply the item's effect to a player.
     * @param player The player the effect will be applied to.
     */
    void applyEffect(Player &player) const override;
};


#endif //PLAY_TOGETHER_COIN_H
