#include "../../../include/Game/LevelManagers/TextureManager.h"

/**
 * @file TextureManager.cpp
 * @brief Implements the TextureManager class responsible for loading textures.
 */


/* CONSTRUCTORS */

TextureManager::TextureManager(Level *level) : levelPtr(level) {}


/* ACCESSORS */

bool TextureManager::getReloadTexture() const {
    return reloadTexture;
}

std::vector<Texture>& TextureManager::getPlatforms() {
    return platforms;
}

std::vector<SDL_Texture*>& TextureManager::getBackgrounds() {
    return backgrounds;
}

SDL_Texture* TextureManager::getMiddleground() {
    return middleground;
}

std::vector<SDL_Texture*>& TextureManager::getForegrounds() {
    return foregrounds;
}


/* MUTATORS */

void TextureManager::setReloadTexture(bool state) {
    reloadTexture = state;
}


/* METHODS */

bool TextureManager::loadMiddlegroundTexture(SDL_Renderer *renderer) {
    std::string folder_path = std::string(TEXTURES_DIRECTORY) + "world_" + std::to_string(levelPtr->getWorldID()) + "/environment/"; // Get the folder path
    std::string file_path = folder_path + "middlegrounds/level_" + std::to_string(levelPtr->getMapID()) + ".png"; // Get the file path
    middleground = IMG_LoadTexture(renderer, file_path.c_str());
    return middleground != nullptr;
}

void TextureManager::loadPlatformTextures(SDL_Renderer &renderer) {
    platforms.clear();

    std::string folder_path = std::string(TEXTURES_DIRECTORY) + "world_" + std::to_string(levelPtr->getWorldID()) + "/platforms/"; // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = folder_path + "platform_" + std::to_string(i) + ".png"; // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        platforms.emplace_back(*new_texture);
    }
}

bool TextureManager::loadBackgroundTextures(SDL_Renderer &renderer) {
    bool check = true;
    backgrounds.clear();

    std::string folder_path = std::string(TEXTURES_DIRECTORY) + "world_" + std::to_string(levelPtr->getWorldID()) + "/environment/backgrounds/"; // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = folder_path + "background_" + std::to_string(i) + ".png"; // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        backgrounds.emplace_back(new_texture);
        if (new_texture == nullptr) check = false;
    }

    return check;
}

bool TextureManager::loadForegroundTextures(SDL_Renderer &renderer) {
    bool check = true;
    foregrounds.clear();

    std::string folder_path = std::string(TEXTURES_DIRECTORY) + "world_" + std::to_string(levelPtr->getWorldID()) + "/environment/foregrounds/"; // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = folder_path + "foreground_" + std::to_string(i) + ".png"; // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        foregrounds.emplace_back(new_texture);
        if (new_texture == nullptr) check = false;
    }

    return check;
}

void TextureManager::loadWorldTextures(SDL_Renderer *renderer) {
    loadPlatformTextures(*renderer);
    loadBackgroundTextures(*renderer);
    loadForegroundTextures(*renderer);
}