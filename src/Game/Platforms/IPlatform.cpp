#include "../../../include/Game/Platforms/IPlatform.h"

/**
 * @file IPlatform.cpp
 * @brief Implements the IPlatform interface responsible for platforms logic.
 */

std::vector<Texture> IPlatform::textures; /**< The textures of all loaded platforms. */

void IPlatform::loadTextures(SDL_Renderer &renderer, int world_id) {
    textures.clear();

    std::string folder_path = std::string(TEXTURES_DIRECTORY) + "world_" + std::to_string(world_id) + "/platforms/"; // Get the folder path

    // Load all the textures of the platforms
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {

        std::string file_path = folder_path + entry.path().filename().string(); // Get the file path

        // Check if the file is a regular file
        if (entry.is_regular_file()) {

            // Check if the file is a png file
            if (entry.path().extension() == ".png") {
                // Load the texture and add it to the vector
                SDL_Texture *new_texture = IMG_LoadTexture(&renderer, file_path.c_str());
                textures.emplace_back(*new_texture);
            }
            else {
                std::cerr << "Error loading texture: '" << entry.path().filename() << "' is not a png file." << std::endl;
            }
        }
    }
}
