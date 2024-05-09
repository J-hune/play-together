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
        std::cerr << "Layer: 0 is a forbidden value, change the value or use a Texture instead." << std::endl;
        exit(1);
    }
}


/* ACCESSORS */

int Layer::getLayer() const {
    return layer;
}


/* METHODS */

void Layer::render(SDL_Renderer *renderer, const Point *camera) const {
    SDL_Rect src_rect = getSize();
    int position_index = (static_cast<int>(camera->x * ratio) / src_rect.w);

    auto is_even = position_index % 2 == 0;
    auto x1 = static_cast<float>(src_rect.w * (is_even ? position_index : (position_index + 1)));
    auto x2 = static_cast<float>(src_rect.w * (is_even ? (position_index + 1) : position_index));

    // Rendering 1st image
    SDL_FRect layer_rect_1 = { x1 - camera->x * ratio, -250, static_cast<float>(src_rect.w), static_cast<float>(src_rect.h)}; // TODO: change '-250' value
    SDL_RenderCopyExF(renderer, getTexture(), &src_rect, &layer_rect_1, 0.0, nullptr, getFlip());

    // Rendering 2nd image
    SDL_FRect layer_rect_2 = {x2 - camera->x * ratio, -250, static_cast<float>(src_rect.w), static_cast<float>(src_rect.h)}; // TODO: change '-250' value
    SDL_RenderCopyExF(renderer, getTexture(), &src_rect, &layer_rect_2, 0.0, nullptr, getFlip());
}