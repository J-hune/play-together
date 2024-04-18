#ifndef PLAY_TOGETHER_TEXTUREMANAGER_H
#define PLAY_TOGETHER_TEXTUREMANAGER_H

#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include "../../../dependencies/json.hpp"
#include "../../Graphics/Texture.h"


/**
 * @file TextureManager.h
 * @brief Defines the TextureManager class responsible for loading textures.
 */



class TextureManager {
private:
    int worldID = -1; /**< Represents the ID of the world from which textures are loaded. */

    std::vector<Texture> platforms; /**< Collection of Texture representing the platforms. */
    std::vector<SDL_Texture*> backgrounds; /**< Collection of SDL_Texture representing the background textures. */
    SDL_Texture *middleground; /**< SDL_Texture representing the middle ground texture. */
    std::vector<SDL_Texture*> foregrounds; /**< Collection of SDL_Texture representing the foreground textures. */



public:
    /* CONSTRUCTORS */

    TextureManager() = default;


    /* ACCESSORS */

    [[nodiscard]] int getWorldID() const;
    [[nodiscard]] std::vector<Texture>& getPlatforms();
    [[nodiscard]] std::vector<SDL_Texture*>& getBackgrounds();
    [[nodiscard]] SDL_Texture* getMiddleground();
    [[nodiscard]] std::vector<SDL_Texture*>& getForegrounds();


    /* METHODS */

    /**
     * @brief Load the middleground texture.
     * @param renderer Represents the renderer of the game.
     * @param level_id Represents the ID of the level.
     * @return True if the texture was loaded successfully, false otherwise.
     */
    bool loadMiddlegroundTexture(SDL_Renderer *renderer, int level_id);

    /**
     * @brief Load the textures of the world.
     * @param renderer Represents the renderer of the game.
     * @param world_id Represents the ID of the world to load.
     */
    void loadWorldTextures(SDL_Renderer *renderer, int world_id);

private:

    /**
     * @brief Load the textures of the platforms.
     */
    void loadPlatformTextures(SDL_Renderer &renderer);

    /**
     * @brief Load the background textures.
     * @param renderer Represents the renderer of the game.
     * @return True if the textures were loaded successfully, false otherwise.
     */
    bool loadBackgroundTextures(SDL_Renderer &renderer);

    /**
     * @brief Load the middleground textures.
     * @param renderer Represents the renderer of the game.
     * @return True if the textures were loaded successfully, false otherwise.
     */
    bool loadForegroundTextures(SDL_Renderer &renderer);



};


#endif //PLAY_TOGETHER_TEXTUREMANAGER_H
