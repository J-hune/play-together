#ifndef PLAY_TOGETHER_TEXTUREMANAGER_H
#define PLAY_TOGETHER_TEXTUREMANAGER_H

#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "../Level.h"


// Forward declaration of Level
class Level;

class TextureManager {
private:
    bool reloadTexture = true; /**< Flag indicating if the world textures must be reloaded. */

    std::vector<Texture> platforms; /**< Collection of Texture representing the platforms. */
    std::vector<SDL_Texture*> backgrounds; /**< Collection of SDL_Texture representing the background textures. */
    SDL_Texture *middleground; /**< SDL_Texture representing the middle ground texture. */
    std::vector<SDL_Texture*> foregrounds; /**< Collection of SDL_Texture representing the foreground textures. */

    Level *levelPtr; /**< A pointer to the level object. */

public:

    /* CONSTRUCTORS */

    explicit TextureManager(Level *level);


    /* ACCESSORS */

    [[nodiscard]] bool getReloadTexture() const;
    [[nodiscard]] std::vector<Texture>& getPlatforms();
    [[nodiscard]] std::vector<SDL_Texture*>& getBackgrounds();
    [[nodiscard]] SDL_Texture* getMiddleground();
    [[nodiscard]] std::vector<SDL_Texture*>& getForegrounds();

    /* MUTATORS */

    void setReloadTexture(bool state);


    /* METHODS */

    /**
     * @brief Load the middleground texture.
     * @param renderer Represents the renderer of the game.
     * @return True if the texture was loaded successfully, false otherwise.
     */
    bool loadMiddlegroundTexture(SDL_Renderer *renderer);

    /**
     * @brief Load the textures of the world.
     * @param renderer Represents the renderer of the game.
     */
    void loadWorldTextures(SDL_Renderer *renderer);

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
