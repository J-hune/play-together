#ifndef PLAY_TOGETHER_MUSIC_H
#define PLAY_TOGETHER_MUSIC_H

#include <SDL_mixer.h>
#include <string>
#include <iostream>

// Define constants for directories and file names
constexpr char MUSICS_DIRECTORY[] = "assets/musics/";

/**
 * @file Music.h
 * @brief Defines the Music class responsible for music object.
 */

/**
 * @class Music
 * @brief Represents the music object.
 */
class Music {
private:
    /* ATTRIBUTES */

    Mix_Music* music; /**< The music file to be played. */

public:

    static int volume; /**< The music volume. */


    /* CONSTRUCTORS */

    Music() = default;
    explicit Music(const std::string& file_name);
    ~Music();


    /* METHODS */

    /**
     * @brief Stop playing the music.
     */
    static void stop();

    /**
     * @brief Set a new volume to the music.
     * @param volume An int value between 0 and 128.
     */
    static void setVolume(int new_volume);

    /**
     * @brief Start playing the music.
     * @param loop Number of time the music will be played, -1 to loop the music forever.
     */
    void play(int loop);

    /**
     * @brief Pause or play the music.
     */
    void togglePause();


};


#endif //PLAY_TOGETHER_MUSIC_H
