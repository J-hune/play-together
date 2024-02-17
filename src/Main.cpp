#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#include <mutex>
#include <SDL_ttf.h>
#include <thread>
#include "../include/Game/Game.h"
#include "../include/Utils/ApplicationConsole.h"
#include "../include/Graphics/Button.h"
#include "../include/Game/Menu.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL2: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "Error initializing SDL2_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    /*
    window = SDL_CreateWindow(
            "Play Together",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN
    );*/

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Load font from a TrueType (TTF) file
    TTF_Font* font = TTF_OpenFont("../assets/font/arial.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Initialize Game
    Player initialPlayer(50, 50, 2, 20, 30);
    Game game(window, renderer, initialPlayer);

    // Initialize Menu
    Menu menu(renderer, font, &game);
    menu.render();

    // Initialize Application Console
    ApplicationConsole console(&game);
    std::jthread consoleThread(&ApplicationConsole::Run, &console);
    consoleThread.detach();

    // Main loop
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            // If the escape key is pressed, stop the game
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                menu.setDisplayMenu(true);
            }

            else {
                // Handle menu events
                menu.handleEvent(event);
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // If the game should start
        if (!menu.isDisplayingMenu()) {
            // Create and start the game
            Player character1(100, 50, 2, 20, 30);
            Player character2(150, 50, 2, 20, 30);
            Player character3(200, 50, 2, 20, 30);

            game.addCharacter(character1);
            game.addCharacter(character2);
            game.addCharacter(character3);

            game.loadPolygonsFromMap("experimentation");

            game.removeCharacter(character3);

            // Launch the game loop in a separate thread
            std::jthread gameThread(&Game::run, &game);
            gameThread.join();

            // Reset the menu after exiting the game
            menu.reset();
        } else {
            // Render the menu
            menu.render();
        }

        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
