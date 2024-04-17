#include "../../../include/Game/GameManagers/TextureManager.h"

/**
 * @file TextureManager.cpp
 * @brief Implements the TextureManager class responsible for loading textures.
 */


/* ACCESSORS */

int TextureManager::getWorldID() const {
    return worldID;
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


/* METHODS */

bool TextureManager::loadMiddlegroundTexture(SDL_Renderer *renderer, int level_id) {
    std::string folder_path = std::format("{}world_{}/environment/", TEXTURES_DIRECTORY, level_id); // Get the folder path
    std::string file_path = std::format("{}middlegrounds/level_{}.png", folder_path, level_id); // Get the file path
    middleground = IMG_LoadTexture(renderer, file_path.c_str());
    return middleground != nullptr;
}

void TextureManager::loadPlatformTextures(SDL_Renderer &renderer) {
    platforms.clear();

    std::string folder_path = std::format("{}world_{}/platforms/", TEXTURES_DIRECTORY, worldID); // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = std::format("{}platform_{}.png", folder_path, i); // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        platforms.emplace_back(*new_texture);
    }
}

bool TextureManager::loadBackgroundTextures(SDL_Renderer &renderer) {
    bool check = true;
    backgrounds.clear();

    std::string folder_path = std::format("{}world_{}/environment/backgrounds/", TEXTURES_DIRECTORY, worldID); // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = std::format("{}background_{}.png", folder_path, i); // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        backgrounds.emplace_back(new_texture);
        if (new_texture == nullptr) check = false;
    }

    return check;
}

bool TextureManager::loadForegroundTextures(SDL_Renderer &renderer) {
    bool check = true;
    foregrounds.clear();

    std::string folder_path = std::format("{}world_{}/environment/foregrounds/", TEXTURES_DIRECTORY, worldID); // Get the folder path

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            file_count++;
        }
    }

    // Load all the textures of the backgrounds
    for (int i = 0; i < file_count; i++) {
        std::string file_path = std::format("{}foreground_{}.png", folder_path, i); // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        foregrounds.emplace_back(new_texture);
        if (new_texture == nullptr) check = false;
    }

    return check;
}

void TextureManager::loadWorldTextures(SDL_Renderer *renderer, int world_id) {
    worldID = world_id;
    loadPlatformTextures(*renderer);
    loadBackgroundTextures(*renderer);
    loadForegroundTextures(*renderer);
}