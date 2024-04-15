#ifndef PLAY_TOGETHER_LAYER_H
#define PLAY_TOGETHER_LAYER_H

#include <cmath>
#include "Texture.h"
#include "../Game/Point.h"


/**
 * @file Layer.h
 * @brief Defines the Layer class representing a layered texture in a 2D game.
 */

/**
 * @class Layer
 * @brief Represents a layered texture in a 2D game.
 */
class Layer : public Texture {
private:
    /* ATTRIBUTES */

    int layer; /**< The index of the layer, 1 is the middleground, 0 is static, negatives are background, positives are foreground. */
    float ratio; /**< The ratio of the layer. */

public:
    /* CONSTRUCTORS */

    Layer() = default;
    Layer(SDL_Texture &texture, int layer);


    /* ACCESSORS */

    /**
     * @brief Returns the layer index.
     * @return The layer index.
     */
    [[nodiscard]] int getLayer() const;


    /* METHODS */

    /**
     * @brief Renders the layer texture.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void render(SDL_Renderer *renderer, const Point *camera) const;
};


#endif //PLAY_TOGETHER_LAYER_H
