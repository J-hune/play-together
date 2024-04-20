#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */


/* CONSTRUCTORS */

Game::Game(SDL_Window *window, SDL_Renderer *renderer, int frameRate, bool *quitFlag, MessageQueue *messageQueue)
        : window(window), renderer(renderer), frameRate(frameRate), quitFlagPtr(quitFlag), messageQueue(messageQueue) {

    // Initialize managers
    inputManager = std::make_unique<InputManager>(this);
    renderManager = std::make_unique<RenderManager>(renderer, this);
    textureManager = std::make_unique<TextureManager>();
    saveManager = std::make_unique<SaveManager>(this);
    broadPhaseManager = std::make_unique<BroadPhaseManager>(this);
    playerManager = std::make_unique<PlayerManager>(this);
    playerCollisionManager = std::make_unique<PlayerCollisionManager>(this);
    eventCollisionManager = std::make_unique<EventCollisionManager>(this);

    // Create the game seed
    std::random_device rd;
    seed = rd();
}


/* ACCESSORS */

GameState Game::getGameState() const {
    return gameState;
}

InputManager &Game::getInputManager() {
    return *inputManager;
}

TextureManager &Game::getTextureManager() {
    return *textureManager;
}

RenderManager &Game::getRenderManager() {
    return *renderManager;
}

SaveManager &Game::getSaveManager() {
    return *saveManager;
}

PlayerManager &Game::getPlayerManager() {
    return *playerManager;
}

BroadPhaseManager &Game::getBroadPhaseManager() {
    return *broadPhaseManager;
}

Camera *Game::getCamera() {
    return &camera;
}

Level *Game::getLevel() {
    return &level;
}

int Game::getEffectiveFrameRate() const {
    return effectiveFrameFps;
}

Uint32 Game::getPlaytime() {
    playtime += SDL_GetTicks() - lastPlaytimeUpdate;
    lastPlaytimeUpdate = SDL_GetTicks();
    return playtime;
}


/* MODIFIERS */

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name, renderer, textureManager.get());
}

void Game::setFrameRate(int fps) {
    frameRate = fps;
}

void Game::setPlaytime(Uint32 new_playtime) {
    lastPlaytimeUpdate = SDL_GetTicks();
    playtime = new_playtime;
}


/* METHODS */

void Game::initializeHostedGame(int slot) {
    saveManager->setSlot(slot);

    // Try to load the game state from the save slot
    if (!saveManager->loadGameState()) {
        setPlaytime(0);
        level = Level("diversity", renderer, textureManager.get());
        std::cout << "Game: No save file found in slot " << slot << ", starting new game at level: " << level.getMapName() << std::endl;
    }

    // Add the initial player to the game
    Point spawnPoint = level.getSpawnPoints(level.getLastCheckpoint())[0];

    Player initialPlayer(-1, spawnPoint, 2);
    camera.initializePosition(spawnPoint);
    music = level.getMusicById(0);
    music.play(-1);

    Asteroid::generateRandomAnglesArray(200, seed);
    Asteroid::generateRandomPositionsArray(200, 0, camera.getW(), seed);

    initialPlayer.setSpriteTextureByID(2);
    playerManager->addPlayer(initialPlayer);
}

void Game::loadLevel(const std::string &map_name, short last_checkpoint, const nlohmann::json::array_t &players) {
    setLevel(map_name);
    level.setLastCheckpoint(last_checkpoint);

    // Add all players to the game (including the local player)
    size_t spawnIndex = 0;
    for (const auto &player: players) {
        int receivedPlayerID = player["playerID"];

        Point spawnPoint = level.getSpawnPoints(last_checkpoint)[spawnIndex];
        Player newPlayer(receivedPlayerID, spawnPoint, 2);
        if (player.contains("x")) newPlayer.setX(player["x"]);
        if (player.contains("y")) newPlayer.setY(player["y"]);
        if (player.contains("moveX")) newPlayer.setMoveX(player["moveX"]);
        if (player.contains("moveY")) newPlayer.setMoveY(player["moveY"]);

        if (receivedPlayerID == -1) newPlayer.setSpriteTextureByID(3);
        else if (receivedPlayerID == 0) newPlayer.setSpriteTextureByID(2);

        playerManager->addPlayer(newPlayer);
        spawnIndex++;

        // Enter the game loop
        Mediator::setDisplayMenu(false);
    }

    music = level.getMusicById(0);
    music.play(-1);
}

void Game::update(double delta_time) {
    inputManager->handleKeyboardEvents();
    calculatePlayersMovement(delta_time);
    if (level.applyTrapsMovement(delta_time)) camera.setShake(150);

    level.applyPlatformsMovement(delta_time);
    level.applyAsteroidsMovement(delta_time);
    applyPlayersMovement(delta_time);
    camera.applyMovement(playerManager->getAveragePlayerPosition(), delta_time);

    // Handle collisions
    broadPhaseManager->broadPhase();
    narrowPhase(delta_time);
    playerManager->setTheBestPlayer();
    if (!Mediator::isClientRunning()) level.generateAsteroid(0, {camera.getX(), camera.getY()}, seed);
    renderManager->render();
}

void Game::run() {
    gameState = GameState::RUNNING;

    // Variables for controlling FPS and calculating delta time
    Uint64 lastFrameTime = SDL_GetPerformanceCounter(); // Time at the start of the game frame
    Uint64 frequency = SDL_GetPerformanceFrequency();
    double accumulatedTime = 0.0; // Accumulated time since last effective game FPS update
    int frameCounter = 0;

    double elapsedTimeSinceLastReset = 0.0; // Time elapsed since last reset

    // Game loop
    while (gameState != GameState::STOPPED) {

        // Process messages from other threads in the queue
        if (!messageQueue->empty()) {
            auto [mainMessage, parameters] = messageQueue->pop();

            if (mainMessage == "InitializeClientGame") {
                nlohmann::json message = nlohmann::json::parse(parameters[0]);
                loadLevel(message["mapName"], message["lastCheckpoint"], message["players"]);
            }
        }

        // Calculate delta time for game logic
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        Uint64 frameTicks = currentFrameTime - lastFrameTime;
        float delta_time = static_cast<float>(frameTicks) / static_cast<float>(frequency); // Delta time in seconds for game logic
        lastFrameTime = currentFrameTime;

        // Accumulate time for game logic and rendering
        accumulatedTime += delta_time;
        elapsedTimeSinceLastReset += delta_time;

        // Calculate game rendering at the specified rate (frameRate)
        if (accumulatedTime >= 1.0 / frameRate) {
            frameCounter++;
            update(delta_time);

            // Every 1/60 seconds or more, send the keyboard state to the network
            if (elapsedTimeSinceLastReset > networkInputSendIntervalSeconds) {
                inputManager->sendKeyboardStateToNetwork();
            }

            // Every 20 seconds or more, send the sync correction to the network
            if (Mediator::isServerRunning() && elapsedTimeSinceLastReset > networkSyncCorrectionIntervalSeconds) {
                inputManager->sendSyncCorrectionToNetwork();
            }

            // Check if one second has passed since the last reset, and if so, reset frame counters and elapsed time
            if (elapsedTimeSinceLastReset >= effectiveFrameRateUpdateIntervalSeconds) {
                effectiveFrameFps = frameCounter;
                frameCounter = 0;
                elapsedTimeSinceLastReset -= 1.0;
            }

            // Reset accumulated time for rendering
            accumulatedTime -= 1.0 / frameRate;
        }

        // Waiting to maintain the desired game FPS
        Uint64 desiredTicksPerFrame = frequency / frameRate;
        Uint64 elapsedGameTicks = SDL_GetPerformanceCounter() - currentFrameTime;
        Uint64 ticksToWait = desiredTicksPerFrame > elapsedGameTicks ? desiredTicksPerFrame - elapsedGameTicks : 0;
        SDL_Delay(static_cast<Uint32>(ticksToWait * 1000 / frequency));
    }
}

void Game::calculatePlayersMovement(double delta_time) {
    // Apply movement to all players
    for (Player &player: playerManager->getAlivePlayers()) {
        player.calculateMovement(delta_time);
    }
}

void Game::applyPlayersMovement(double delta_time) {
    // Apply movement for all players
    for (Player &player: playerManager->getAlivePlayers()) {
        player.applyMovement(delta_time);
    }
}

void Game::switchMavity() {
    // Change mavity for all players
    for (Player &player: playerManager->getAlivePlayers()) {
        player.setIsGrounded(false);
        player.getSprite()->toggleFlipVertical();
        player.toggleMavity();
    }
}

void Game::narrowPhase(double delta_time) {
    eventCollisionManager->handleAsteroidsCollisions(); // Handle collisions for asteroids
    playerCollisionManager->handleCollisions(delta_time); // Handle collisions for all players
}

void Game::togglePause() {
    using enum GameState;
    if (gameState == PAUSED) {
        Mediator::setDisplayMenu(false);
        gameState = RUNNING;
        Music::setVolume(Music::volume);
        SoundEffect::masterVolume = 20;
    } else {
        // For each key, if it is pressed, call handleKeyUpEvent
        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
            if (keyboardState[i]) {
                SDL_KeyboardEvent keyEvent;
                keyEvent.keysym.scancode = static_cast<SDL_Scancode>(i);
                inputManager->handleKeyUpEvent(playerManager->findPlayerById(-1), keyEvent);
            }
        }

        Music::setVolume(1);
        SoundEffect::masterVolume = 1;
        gameState = PAUSED;
        Mediator::setDisplayMenu(true);
    }
}

void Game::stop() {
    gameState = GameState::STOPPED;
    playerManager->clearPlayers();
    saveManager->setSlot(-1);

    // Reset music
    Music::stop();
    SoundEffect::stop();
    Music::setVolume(Music::volume);
    SoundEffect::masterVolume = 20;
}

void Game::exitGame() {
    *quitFlagPtr = true;
}