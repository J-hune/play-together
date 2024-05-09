#include "../../include/Sounds/Music.h"

/**
 * @file Music.cpp
 * @brief Implements the Music class responsible for music object.
 */

// Static member initialization
int Music::volume = 20;


/* CONSTRUCTORS */

Music::Music(const std::string& file_name) {
    std::string file_path = std::string(MUSICS_DIRECTORY) + file_name;
    music = Mix_LoadMUS(file_path.c_str());

    // Check error
    if (music == nullptr) {
        std::cerr << "Music: " << Mix_GetError() << std::endl;
    }
}

/* METHODS */

void Music::stop() {
    Mix_HaltMusic();
}

void Music::setVolume(int new_volume) {
    Mix_VolumeMusic(new_volume);
}

void Music::play(int loop) {
    Mix_PlayMusic(music, loop);
    setVolume(volume);
}

void Music::togglePause() {
    if (Mix_PausedMusic()) Mix_ResumeMusic();
    else Mix_PauseMusic();
}