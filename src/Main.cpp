// main.cpp
#include "../include/Game/Game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Collision Detection", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player initialPlayer(50, 50, 2, 20, 30);
    Game game(window, renderer, initialPlayer);
    game.loadPolygonsFromMap("experimentation");

    game.run();

    return 0;}
