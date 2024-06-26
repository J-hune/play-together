#ifndef PLAY_TOGETHER_ITEM_H
#define PLAY_TOGETHER_ITEM_H

#include "../Player.h"
#include "../../Sounds/SoundEffect.h"

/**
 * @file Item.h
 * @brief Defines the item class responsible for item object.
 */

enum class ItemTypes {
    SIZE_POWER_UP,
    SPEED_POWER_UP,
    COIN
};

/**
 * @class Item
 * @brief Represents the item object.
 */
class Item {
private :
    /* ATTRIBUTES */

    float x; /**< The x-coordinate of the item's position. */
    float y; /**< The y-coordinate of the item's position. */
    float width; /**< The width of the item. */
    float height; /**< The height of the item. */
    bool isOnScreen = true; /**< Flag indicating if the item is on screen. */
    SoundEffect collectSound; /**< The sound effect associated to the item. */

public:
    /* CONSTRUCTORS */

    Item(float X, float Y, float width, float height, const std::string& file_name);
    virtual ~Item() = default;


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
     * @brief Return the width attribute.
     * @return The value of the width attribute.
     */
    [[nodiscard]] float getHeight() const;

    /**
     * @brief Return the height attribute.
     * @return The value of the height attribute.
     */
    [[nodiscard]] float getWidth() const;

    /**
     * @brief Get the bounding box of the platform.
     * @return SDL_Rect representing the platform box.
     */
    [[nodiscard]] SDL_FRect getBoundingBox() const;

    /**
     * @brief Overloaded equality operator.
     * @param item The item object to compare with.
     * @return True if the items are equal, false otherwise.
     */
    bool operator==(const Item& item) const;


    /* MUTATORS */

    /**
     * @brief Set the isOnScreen attribute.
     * @param state The new state of the isOnScreen attribute.
     */
    void setIsOnScreen(bool state);


    /* METHODS */

    /**
     * @brief Apply the item's effect to a player.
     * @param player The player the effect will be applied to.
     */
    virtual void applyEffect(Player &player) = 0;

    /**
     * @brief Apply the item's inverse effect to a player.
     * @param player The player the effect will be applied to.
     */
    virtual void inverseEffect(Player &player) = 0;

    /**
     * @brief Pure virtual method to renders the item's sprite.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    virtual void render(SDL_Renderer *renderer, Point camera) = 0;

    /**
     * @brief Renders the collisions by drawing a rectangle.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderDebug(SDL_Renderer *renderer, Point camera) const;

};


#endif //PLAY_TOGETHER_ITEM_H
