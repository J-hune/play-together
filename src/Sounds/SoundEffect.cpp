#include "../../include/Sounds/SoundEffect.h"

/**
 * @file SoundEffect.cpp
 * @brief Implements the SoundEffect class responsible for sound effect object.
 */

// Static member initialization
int SoundEffect::masterVolume = 32;


/* CONSTRUCTORS */

SoundEffect::SoundEffect(const std::string& file_name) {
    std::string file_path = std::string(SOUNDS_DIRECTORY) + file_name;
    sound = Mix_LoadWAV(file_path.c_str());

    // Check error
    if (sound == nullptr) {
        std::cerr << "SoundEffect: " << Mix_GetError() << std::endl;
    } else {
        setVolume(volume);
    }
}

SoundEffect::SoundEffect(const std::string& file_name, int volume) : volume(volume) {
    std::string file_path = std::string(SOUNDS_DIRECTORY) + file_name;
    sound = Mix_LoadWAV(file_path.c_str());

    // Check error
    if (sound == nullptr) {
        std::cerr << "SoundEffect: " << Mix_GetError() << std::endl;
    } else {
        setVolume(volume);
    }
}


/* ACCESSORS */

int SoundEffect::getChannelNumber() {
    return MIX_CHANNELS;
}

int SoundEffect::getMasterVolume() {
    return Mix_Volume(-1, -1);
}

int SoundEffect::getVolume() const {
    return volume;
}


/* MUTATORS */

void SoundEffect::setVolume(int new_volume) {
    Mix_VolumeChunk(sound, new_volume);
}


/* METHODS */

void SoundEffect::stop() {
    Mix_HaltChannel(-1);
}

void SoundEffect::play(int loop, int vol) {
    Mix_PlayChannel(-1, sound, loop); // '-1' takes the first available channel
    setVolume(vol < 0 ? masterVolume : vol);
}

