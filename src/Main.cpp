#include <thread>
#include <SDL_ttf.h>
#include "../include/Utils/ApplicationConsole.h"
#include "../include/Graphics/Button.h"
#include "../include/Game/Menu.h"
#include "../include/Network/NetworkManager.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
#endif

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
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int numDisplays = SDL_GetNumVideoDisplays();
    if (numDisplays < 1) {
        fprintf(stderr, "Could not get number of displays: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Get the maximum refresh rate of the display list (in Hz)
    int maxFrameRate = 0;
    for (int i = 0; i < numDisplays; i++) {
        SDL_DisplayMode displayMode;
        if (SDL_GetCurrentDisplayMode(i, &displayMode) == 0) {
            maxFrameRate = std::max(maxFrameRate, displayMode.refresh_rate);
        } else {
            fprintf(stderr, "Could not get display mode: %s\n", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }

    // Load font from a TrueType (TTF) file
    std::vector<TTF_Font*> fonts;
    TTF_Font *font16 = TTF_OpenFont("../assets/font/arial.ttf", 16);
    TTF_Font *font24 = TTF_OpenFont("../assets/font/arial.ttf", 24);
    fonts.push_back(font16);
    fonts.push_back(font24);

    for (TTF_Font const* font : fonts) {
        if (font == nullptr) {
            std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
            return 1;
        }
    }

    // Define a boolean to control the game loop
    bool quit = false;

    Mediator mediator;

    // Initialize Game
    Camera camera = Camera();
    Level level("diversity");
    Player::loadTextures(*renderer);
    Player initialPlayer(50, 50, 48, 36);

    Game game(window, renderer, maxFrameRate, fonts, camera, level, initialPlayer);
    mediator.setGamePtr(&game);

    // Initialize Menu
    Menu menu(renderer, fonts, &game, &quit, &mediator);
    mediator.setMenuPtr(&menu);
    menu.render();

    // Initialize NetworkManager
    NetworkManager networkManager(&mediator);
    mediator.setNetworkManagerPtr(&networkManager);

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
            Player character1(100, 50, 48, 36);
            Player character2(150, 50, 48, 36);
            Player character3(200, 50, 48, 36);

            //game.addCharacter(character1);
            //game.addCharacter(character2);
            game.addCharacter(character3);

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

    for (TTF_Font* font : fonts) {
        TTF_CloseFont(font);
    }

    TTF_Quit();
    SDL_Quit();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
