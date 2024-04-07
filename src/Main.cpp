#include <thread>
#include <SDL_ttf.h>
#include "../include/Utils/ApplicationConsole.h"
#include "../include/Graphics/Button.h"
#include "../include/Game/Menu.h"
#include "../include/Network/NetworkManager.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {

// Initialize Winsock on Windows
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

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Get the maximum refresh rate of the display list (in Hz)
    int maxFrameRate = 30;
    int numDisplays = SDL_GetNumVideoDisplays();
    if (numDisplays < 1) {
        fprintf(stderr, "Could not get number of displays: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (int i = 0; i < numDisplays; i++) {
        SDL_DisplayMode displayMode;
        if (SDL_GetCurrentDisplayMode(i, &displayMode) != 0) {
            fprintf(stderr, "Could not get display mode: %s\n", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        maxFrameRate = std::max(maxFrameRate, displayMode.refresh_rate);
    }

    // Load fonts
    std::vector<TTF_Font*> fonts;
    TTF_Font *font16 = TTF_OpenFont("assets/font/arial.ttf", 16);
    TTF_Font *font24 = TTF_OpenFont("assets/font/arial.ttf", 24);
    fonts.push_back(font16);
    fonts.push_back(font24);
    for (TTF_Font const* font : fonts) {
        if (font == nullptr) {
            std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
            return 1;
        }
    }

    // Initialize game objects
    bool quit = false;
    if (!Player::loadTextures(*renderer)) {
        std::cerr << "Error loading player textures" << std::endl;
        return 1;
    }

    Asteroid::loadTextures(*renderer);
    Asteroid::generateArrayAngles(200);
    Asteroid::generateArrayPositions(200, 0, camera.getW());

    Game game(window, renderer, maxFrameRate, fonts, &quit);
    Menu menu(renderer, fonts, &quit);
    NetworkManager networkManager;
    ApplicationConsole console(&game);

    // Initialize pointers for communication between objects
    Mediator::setGamePtr(&game);
    Mediator::setMenuPtr(&menu);
    Mediator::setNetworkManagerPtr(&networkManager);

    // Start console thread
    std::jthread consoleThread(&ApplicationConsole::run, &console);
    consoleThread.detach();

    Uint64 lastFrameTime = SDL_GetPerformanceCounter();

    // Main loop
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
                continue;
            }

            // Handle menu events
            menu.handleEvent(event);
        }

        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());
        lastFrameTime = currentFrameTime;

        // Limit the frame rate
        if (deltaTime < 1000.0f / static_cast<float>(maxFrameRate)) {
            SDL_Delay(static_cast<Uint32>((1000.0f / static_cast<float>(maxFrameRate)) - deltaTime));
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // If the game should start
        if (!menu.isDisplayingMenu()) {
            game.run(); // Block the main thread until the game is finished
            menu.reset(); // Reset the menu after exiting the game
        } else {
            // Render the menu
            menu.render();
            SDL_RenderPresent(renderer);
        }
    }

    /* Clean up resources */
    networkManager.stopServers();
    networkManager.stopClients();
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
