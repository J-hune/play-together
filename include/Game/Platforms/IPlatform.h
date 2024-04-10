#ifndef PLAY_TOGETHER_IPLATFORM_H
#define PLAY_TOGETHER_IPLATFORM_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <SDL_image.h>
#include "../Point.h"
#include "../../Graphics/Texture.h"

/**
 * @file Platform.h
 * @brief Defines the Platform interface representing a platform.
 */

/**
 * @interface IPlatform
 * @brief Represents a platform in a 2D game.
 */
class IPlatform {
public:

    static std::vector<Texture> textures; /**< The textures of all loaded platforms. */

    // VIRTUAL DESTRUCTOR
    virtual ~IPlatform() = default;

    // ACCESSORS
    [[nodiscard]] virtual float getX() const = 0;
    [[nodiscard]] virtual float getY() const = 0;
    [[nodiscard]] virtual float getW() const = 0;
    [[nodiscard]] virtual float getH() const = 0;
    [[nodiscard]] virtual bool getIsMoving() const = 0;
    [[nodiscard]] virtual SDL_FRect getBoundingBox() const = 0;

    // ACCESSORS
    virtual void setIsMoving(bool state) = 0;

    // METHODS
    virtual void applyMovement(double delta_time) = 0;
    virtual void render(SDL_Renderer *renderer, Point camera) const = 0;
    virtual void renderDebug(SDL_Renderer *renderer, Point camera) const = 0;

    /**
     * @brief Load the textures of the platforms.
     * @param world_id The id of the world.
     */
    static void loadTextures(SDL_Renderer &renderer, int world_id);
};


#endif //PLAY_TOGETHER_IPLATFORM_H
