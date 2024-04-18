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
    std::string properties_file_path = std::string(folder_path) + "/properties.json";
    std::ifstream file(properties_file_path);

    if (!file.is_open()) {
        std::cerr << "TextureManager: Unable to open the properties file. Please check the file path." << std::endl;
        exit(1);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    // Count the number of files in the folder
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".png") {
            file_count++;
        }
    }

    // Check if every texture has its properties
    if (file_count > static_cast<int>(j["offsets"].size())) {
        std::cerr << "TextureManager: Missing properties for a platform. Please check the properties file." << std::endl;
        exit(1);
    }

    // Load all the textures of the platforms
    for (int i = 0; i < file_count; i++) {
        std::string file_path = std::format("{}platform_{}.png", folder_path, i); // Get the file path
        SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
        float x = j["offsets"][i][0];
        float y = j["offsets"][i][1];
        float w = j["offsets"][i][2];
        float h = j["offsets"][i][3];
        SDL_FRect texture_offsets = {x, y, x + w, y + h};
        platforms.emplace_back(*new_texture, texture_offsets);
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

    std::cout << "TextureManager: Loaded world textures." << std::endl;
}