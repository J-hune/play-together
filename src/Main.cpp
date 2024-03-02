#include <thread>
#include <mutex>


#include <fstream>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../include/Game/Game.h"
#include "../include/Utils/GameConsole.h"
#include "../include/Utils/Saves.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    /*
    window = SDL_CreateWindow(
            "Play Together",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN
    );*/

    window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }
    /*it worksssssss
    std::string obj = "holla";
    std::ofstream ofs("test");

    {
        boost::archive::text_oarchive oa(ofs);
        oa & obj;
    }

    //std::string newt;
    Game newg();

    {
        // create and open an archive for input
        std::ifstream ifs("test");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> newg;
        // archive and stream closed when destructors are called
    }*/



    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player initialPlayer(-50, 50, 2, 20, 30);

    Player character1(100, 50, 2, 20, 30);
    Player character2(150, 50, 2, 20, 30);
    Player character3(200, 50, 2, 20, 30);

    Game game(window, renderer, initialPlayer);
    GameConsole console(&game);

    game.loadPolygonsFromMap("experimentation");

    game.addCharacter(character1);
    game.addCharacter(character2);
    game.addCharacter(character3);

    game.removeCharacter(character3);

    game.initializeCameraPosition();

    std::ofstream ofs("test");

    {
        boost::archive::text_oarchive oa(ofs);
        oa & game;
    }

    Game ngame(window, renderer, initialPlayer);

    {
        std::ifstream ifs("test");
        boost::archive::text_iarchive ia(ifs);
        ia >> ngame;
    }


    // Launch the game loop in a separate thread
    std::jthread gameThread(&Game::run, &game);

    // Launch the game console in a separate thread
    std::jthread consoleThread(&GameConsole::Run, &console);

    // Wait for the game loop to finish
    gameThread.join();

    // Join the console thread
    consoleThread.join();
    //Saves<Game> saves;
    //saves.save(game,"Game");
    //Saves<std::string>::save("hello","test");



    return 0;
}
