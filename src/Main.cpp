#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#include "../include/Game/Game.h"

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

    window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player initialPlayer(50, 50, 1,2, 20, 30);
    Game game(window, renderer, initialPlayer);
    game.loadPolygonsFromMap("experimentation");

    game.run();

    return 0;
}
