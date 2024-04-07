#include "../../include/Sounds/Music.h"

/**
 * @file Music.cpp
 * @brief Implements the Music class responsible for music object.
 */

/* CONSTRUCTORS */

Music::Music(const char* fileName) {
    std::string filePath = std::string(MAPS_DIRECTORY) + fileName;
    music = Mix_LoadMUS(filePath.c_str());
}

Music::~Music() {
    Mix_FreeMusic(music);
}


/* METHODS */

void Music::play(int loop) {
    Mix_PlayMusic(music, loop);
}

void Music::setVolume(int volume) {
    Mix_VolumeMusic(volume);
}