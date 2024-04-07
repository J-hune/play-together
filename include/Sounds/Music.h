#ifndef PLAY_TOGETHER_MUSIC_H
#define PLAY_TOGETHER_MUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

// Define constants for directories and file names
constexpr char MAPS_DIRECTORY[] = "assets/musics/";

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
    Mix_Music* music;

public:
    /* CONSTRUCTORS */

    explicit Music(const char* fileName);
    ~Music();


    /* METHODS */

    /**
     * @brief Start playing the music.
     * @param loop Number of time the music will be played, -1 to loop the music forever.
     */
    void play(int loop);

    /**
     * @brief Pause the music.
     */
    void pause();

    /**
     * @brief Stop playing the music.
     */
    void stop();

    /**
     * @brief Set a new volume to the music.
     * @param volume An int value between 0 and 128.
     */
    void setVolume(int volume);


};


#endif //PLAY_TOGETHER_MUSIC_H
