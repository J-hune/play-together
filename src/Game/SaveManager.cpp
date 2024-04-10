#include "../../include/Game/SaveManager.h"

/**
 * @file SaveManager.cpp
 * @brief Implements the SaveManager class responsible for saving and loading the game state.
 */


/* CONSTRUCTORS */

SaveManager::SaveManager(Game *game) : gamePtr(game) {}


/* MODIFIERS */

void SaveManager::setSlot(int value) {
    SaveManager::slot = value;
}


/* METHODS */

void SaveManager::saveGameState() {
    Level const &level = gamePtr->getLevel();

    // Create a JSON object to store the game state
    using json = nlohmann::json;
    json game_state_json;

    // Store the game state

    // Save date to format YYYY-MM-DD (local time) using the C++ standard library
    auto now = std::chrono::system_clock::now();
    auto current_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time = {};
#ifdef _WIN32
    localtime_s(&local_time, &current_time);
#else
    localtime_r(&current_time, &local_time);
#endif

    std::stringstream ss;
    ss << std::put_time(&local_time, "%Y-%m-%d");


    game_state_json["date"] = ss.str();
    game_state_json["level"] = level.getMapName();
    game_state_json["lastCheckpoint"] = level.getLastCheckpoint();

    // Save the player state
    std::string save_file_name = std::format("saves/slot_{}.json", slot);

    // If the "saves" directory does not exist, create it
    if (!std::filesystem::exists(save_file_name)) {
        std::cout << "Game: Creating missing saves directory" << std::endl;
        std::filesystem::create_directory("saves");
    }

    // Open the file for writing
    std::ofstream file(save_file_name);

    // If the file was successfully created, write the game state to the file
    if (file.is_open()) {
        file << game_state_json.dump(4);
        file.close();
        std::cout << "SaveManager: Saved game to slot " << slot << std::endl;
    } else {
        std::cerr << "SaveManager: Error saving game to slot " << slot << std::endl;
    }
}

bool SaveManager::loadGameState() {
    printf("SaveManager: Loading game from slot %d\n", slot);

    // Load the game state from the file
    std::string save_file_name = std::format("saves/slot_{}.json", slot);

    // If the file does not exist, return false
    if (!std::filesystem::exists(save_file_name)) {
        return false;
    }

    // Open the file for reading
    std::ifstream file(save_file_name);

    // If the file was successfully opened, read the game state from the file
    if (file.is_open()) {
        // Read the file content
        std::stringstream buffer;
        buffer << file.rdbuf();

        // Parse the JSON content
        using json = nlohmann::json;
        json game_state_json = json::parse(buffer.str());

        // Load the game state
        Level &level = gamePtr->getLevel();
        level = Level(game_state_json["level"], gamePtr->getRenderManager().getRenderer());
        level.setLastCheckpoint(game_state_json["lastCheckpoint"]);

        std::cout << "SaveManager: Loaded game from slot " << slot << std::endl;
        return true;
    } else {
        std::cerr << "SaveManager: Error loading game from slot " << slot << std::endl;
        return false;
    }
}