#ifndef PLAY_TOGETHER_RENDERMANAGER_H
#define PLAY_TOGETHER_RENDERMANAGER_H

#include <SDL_render.h>
#include "Game.h"

class RenderManager {
    public:

    /** CONSTRUCTOR **/
    RenderManager(SDL_Renderer *renderer, Game *game);

    /** ACCESSORS **/
    static std::vector<TTF_Font *> &getFonts();

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


    /** METHODS **/

    /**
     * @brief Renderer the game.
     */
    void render();


private:
    SDL_Renderer *renderer; /**< The SDL_Renderer to render the game. */
    Game *gamePtr; /**< A pointer to the game object. */
    static std::vector<TTF_Font *> fonts; /**< A vector of TTF_Font objects for rendering text. */

    bool render_textures = true;
    bool render_camera_point = false;
    bool render_camera_area = false;
    bool render_player_colliders = false;
    bool render_fps = false;
};
#endif //PLAY_TOGETHER_RENDERMANAGER_H
