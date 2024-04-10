#include "../../include/Graphics/Layer.h"

/**
 * @file Layer.cpp
 * @brief Implements the Layer class responsible for background and foreground texture logic.
 */

/* CONSTRUCTORS */

Layer::Layer(SDL_Texture &texture, int layer) : Texture(texture), layer(layer) {
    if (layer < 0) {
        ratio = 1 / std::fabs(static_cast<float>(layer));
    } else if (layer > 0) {
        ratio = static_cast<float>(layer) / 1;
    } else {
        ratio = 0;
    }
}


/* ACCESSORS */

int Layer::getLayer() const {
    return layer;
}


/* METHODS */

void Layer::render(SDL_Renderer *renderer, const Point *camera) const {
    SDL_Rect src_rect = getSize();
    SDL_FRect layer_rect = {0 - camera->x * ratio, -250, static_cast<float>(src_rect.w), static_cast<float>(src_rect.h)}; // TODO: change '-250' value
    SDL_RenderCopyExF(renderer, getTexture(), &src_rect, &layer_rect, 0.0, nullptr, getFlip());
}
