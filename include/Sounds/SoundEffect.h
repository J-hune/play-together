#ifndef PLAY_TOGETHER_SOUNDEFFECT_H
#define PLAY_TOGETHER_SOUNDEFFECT_H

#include <SDL_mixer.h>
#include <string>
#include <iostream>

// Define constants for directories and file names
constexpr char SOUNDS_DIRECTORY[] = "assets/sounds/";


/**
 * @file SoundEffect.h
 * @brief Defines the SoundEffect class responsible for sound effect object.
 */

/**
 * @class SoundEffect
 * @brief Represents the sound effect object.
 */
class SoundEffect {
private:
    /* ATTRIBUTES */

    Mix_Chunk* sound; /**< The sound file to be played. */
    int volume = 20; /**< The sound volume. */

public:

    static int masterVolume; /**< The master volume. */


    /* CONSTRUCTORS */

    SoundEffect() = default;
    explicit SoundEffect(const std::string& file_name);
    explicit SoundEffect(const std::string& file_name, int volume);


    /* ACCESSORS */

    /**
     * @brief Return the number of channel in the game.
     * @return An integer representing the number of channel.
     */
    static int getChannelNumber();

    /**
     * @brief Return the master volume of the game.
     * @return An integer representing the master volume.
     */
    static int getMasterVolume();

    /**
     * @brief Return the volume of the sound.
     * @return An integer representing the volume.
     */
    [[nodiscard]] int getVolume() const;


    /* MUTATORS */

    /**
     * @brief Set a new volume to the sound effect.
     * @param volume An int value between 0 and 128.
     */
    void setVolume(int new_volume);


    /* METHODS */

    /**
     * @brief Stop playing all sound effect.
     */
    static void stop();

    /**
     * @brief Start playing the sound effect.
     * @param loop Number of time the sound effect will be played, -1 to loop the sound effect forever.
     * @param volume The volume to play the sound, -1 to play it at master volume.
     */
    void play(int loop, int vol);



};


#endif //PLAY_TOGETHER_SOUNDEFFECT_H
