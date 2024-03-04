#include <thread>
#include <mutex>
#include <SDL_ttf.h>
#include "../include/Game/Game.h"
#include "../include/Utils/ApplicationConsole.h"
#include "../include/Graphics/Button.h"
#include "../include/Game/Menu.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {

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

    SDL_Window *window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

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
    TTF_Font *font = TTF_OpenFont("../assets/font/arial.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Define a boolean to control the game loop
    bool quit = false;

    // Initialize Game
    Camera camera = Camera();
    Player initialPlayer(-50, 50, 2, 20, 30);
    Game game(window, renderer, camera, initialPlayer);

    // Initialize Menu
    Menu menu(renderer, font, &game, &quit);
    menu.render();

    // Initialize Application Console
    ApplicationConsole console(&game);
    std::jthread consoleThread(&ApplicationConsole::run, &console);
    consoleThread.detach();

    // Main loop
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
            camera.initializeCameraPosition(game.getAveragePlayersPositions());

            // Block the main thread until the game is finished
            game.run();

            // Reset the menu after exiting the game
            menu.reset();
        } else {
            // Render the menu
            menu.render();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
