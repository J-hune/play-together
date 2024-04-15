#ifndef PLAY_TOGETHER_RENDERMANAGER_H
#define PLAY_TOGETHER_RENDERMANAGER_H

#include <SDL_render.h>
#include "../Game.h"

/**
 * @file RenderManager.h
 * @brief Defines the RenderManager class responsible for rendering the game.
 */


class RenderManager {
private:
    SDL_Renderer *renderer; /**< The SDL_Renderer to render the game. */
    Game *gamePtr; /**< A pointer to the game object. */
    static std::vector<TTF_Font *> fonts; /**< A vector of TTF_Font objects for rendering text. */

    // Debug rendering attributes
    bool render_textures = true;
    bool render_camera_point = false;
    bool render_camera_area = false;
    bool render_player_colliders = false;
    bool render_fps = false;


public:

    /* CONSTRUCTORS */

    RenderManager(SDL_Renderer *renderer, Game *game);


    /* ACCESSORS */
    SDL_Renderer* getRenderer();
    static std::vector<TTF_Font *> &getFonts();

    /* MUTATORS */
    void setRenderTextures(bool renderTextures);
    void setRenderCameraPoint(bool renderCameraPoint);
    void setRenderCameraArea(bool renderCameraArea);
    void setRenderPlayerColliders(bool renderPlayerColliders);
    void setRenderFps(bool renderFps);

    void toggleRenderTextures();
    void toggleRenderCameraPoint();
    void toggleRenderCameraArea();
    void toggleRenderPlayerColliders();
    void toggleRenderFps();


    /* METHODS */

    /**
     * @brief Renderer the game.
     */
    void render();

};
#endif //PLAY_TOGETHER_RENDERMANAGER_H
