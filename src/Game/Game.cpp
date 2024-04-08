#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, int frameRate, bool *quitFlag)
        : window(window), renderer(renderer), frameRate(frameRate), quitFlagPtr(quitFlag) {

    inputManager = std::make_unique<InputManager>(this);
    renderManager = std::make_unique<RenderManager>(renderer, this);
    saveManager = std::make_unique<SaveManager>(this);
    playerManager = std::make_unique<PlayerManager>(this);

    // Create the game seed
    std::random_device rd;
    seed = rd();
}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

InputManager &Game::getInputManager() {
    return *inputManager;
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

Camera *Game::getCamera() {
    return &camera;
}

Level &Game::getLevel() {
    return level;
}

int Game::getTickRate() const {
    return tickRate;
}

int Game::getEffectiveFrameRate() const {
    return effectiveFrameFps;
}


/** MODIFIERS **/

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name);
}

void Game::setEnablePlatformsMovement(bool state) {
    enable_platforms_movement = state;
}

void Game::setFrameRate(int fps) {
    this->frameRate = fps;
}


/** METHODS **/

void Game::initializeHostedGame(int slot) {
    saveManager->setSlot(slot);

    // Try to load the game state from the save slot
    if (!saveManager->loadGameState()) {
        level = Level("diversity");
        std::cout << "Game: No save file found in slot " << slot << ", starting new game at level: " << level.getMapName() << std::endl;
    }

    // Add the initial player to the game
    Point spawnPoint = level.getSpawnPoints(level.getLastCheckpoint())[0];

    Player initialPlayer(-1, spawnPoint, 48, 36);
    camera.initializePosition(spawnPoint);
    music = level.getMusicById(0);
    music.play(-1);

    Asteroid::generateRandomAnglesArray(200, seed);
    Asteroid::generateRandomPositionsArray(200, 0, camera.getW(), seed);

    initialPlayer.setSpriteTextureByID(2);
    playerManager->addPlayer(initialPlayer);
}

void Game::fixedUpdate() {
    inputManager->handleKeyboardEvents();
    calculatePlayersMovement(1.0 / tickRate);
}

void Game::update(double deltaTime, double ratio) {
    if (enable_platforms_movement) level.applyPlatformsMovement(deltaTime);

    if (!Mediator::isClientRunning()) {
        level.generateAsteroid(0, {camera.getX(), camera.getY()}, seed);
    }

    level.applyAsteroidsMovement(deltaTime);

    // Apply players movement directly with the calculated ratio
    applyPlayersMovement(ratio);

    // Handle collisions
    broadPhase();
    narrowPhase();

    camera.applyMovement(playerManager->getAveragePlayerPosition(), deltaTime);
    renderManager->render();
}

void Game::run() {
    gameState = GameState::RUNNING;

    // Variables for controlling FPS and calculating delta time
    Uint64 lastFrameTime = SDL_GetPerformanceCounter(); // Time at the start of the game frame
    Uint64 frequency = SDL_GetPerformanceFrequency();
    double accumulatedTime = 0.0; // Accumulated time since last effective game FPS update
    double accumulatedTickRateTime = 0.0; // Accumulated time since last game physics update
    int frameCounter = 0;

    double elapsedTimeSinceLastReset = 0.0; // Time elapsed since last reset

    // Game loop
    while (gameState != GameState::STOPPED) {
        // Calculate delta time for game logic
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        Uint64 frameTicks = currentFrameTime - lastFrameTime;
        float deltaTime = static_cast<float>(frameTicks) / static_cast<float>(frequency); // Delta time in seconds for game logic
        lastFrameTime = currentFrameTime;

        // Accumulate time for game logic and rendering
        accumulatedTime += deltaTime;
        accumulatedTickRateTime += deltaTime;
        elapsedTimeSinceLastReset += deltaTime;

        // Calculate game physics at the specified rate (tickRate)
        if (accumulatedTickRateTime >= 1.0 / tickRate) {
            fixedUpdate();

            // Reset accumulated time for game physics
            accumulatedTickRateTime -= 1.0 / tickRate;
        }

        // Calculate game rendering at the specified rate (frameRate)
        if (accumulatedTime >= 1.0 / frameRate) {
            frameCounter++;

            // Calculate the ratio for applying players movement
            double ratio = static_cast<double>(frameTicks) / (static_cast<double>(frequency) / static_cast<double>(tickRate));
            update(deltaTime, ratio);

            // Reset accumulated time for rendering
            accumulatedTime -= 1.0 / frameRate;

            // Check if one second has passed since the last reset, and if so, reset frame counters and elapsed time
            if (elapsedTimeSinceLastReset >= 1.0) {
                effectiveFrameFps = frameCounter;
                frameCounter = 0;
                elapsedTimeSinceLastReset -= 1.0;
            }
        }

        // Waiting to maintain the desired game FPS
        Uint64 desiredTicksPerFrame = frequency / std::max(tickRate, frameRate);
        Uint64 elapsedGameTicks = SDL_GetPerformanceCounter() - currentFrameTime;
        Uint64 ticksToWait = desiredTicksPerFrame > elapsedGameTicks ? desiredTicksPerFrame - elapsedGameTicks : 0;
        SDL_Delay(static_cast<Uint32>(ticksToWait * 1000 / frequency));
    }
}

void Game::calculatePlayersMovement(double deltaTime) {
    // Apply movement to all players
    for (Player &character: playerManager->getAlivePlayers()) {
        character.calculateMovement(deltaTime);
    }
}

void Game::applyPlayersMovement(double ratio) {
    // Apply movement for all players
    for (Player &character: playerManager->getAlivePlayers()) {
        character.applyMovement(ratio);
    }
}

void Game::switchMavity() {
    // Change mavity for all player
    for (Player &character: playerManager->getAlivePlayers()) {
        character.setIsOnPlatform(false);
        character.getSprite()->toggleFlipVertical();
        character.toggleMavity();
    }
}

void Game::broadPhase() {
    // Empty old broad phase elements
    saveZones.clear();
    deathZones.clear();
    obstacles.clear();
    movingPlatforms1D.clear();
    movingPlatforms2D.clear();
    switchingPlatforms.clear();
    sizePowerUp.clear();
    speedPowerUp.clear();

    std::vector<Point> broadPhaseAreaVertices = camera.getBroadPhaseAreaVertices();
    SDL_FRect broadPhaseAreaBoundingBox = camera.getBroadPhaseArea();

    // Check collisions with each save zone
    for (const Polygon &zone: level.getZones(ZoneType::SAVE)) {
        if (checkSATCollision(broadPhaseAreaVertices, zone)) {
            saveZones.push_back(zone);
        }
    }

    // Check collisions with each death zone
    for (const Polygon &zone: level.getZones(ZoneType::DEATH)) {
        if (checkSATCollision(broadPhaseAreaVertices, zone)) {
            deathZones.push_back(zone);
        }
    }

    // Check collisions with each obstacle
    for (const Polygon &obstacle: level.getZones(ZoneType::COLLISION)) {
        if (checkSATCollision(broadPhaseAreaVertices, obstacle)) {
            obstacles.push_back(obstacle);
        }
    }

    // Check for collisions with each 1D moving platforms
    for (const MovingPlatform1D &platform: level.getMovingPlatforms1D()) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, platform.getBoundingBox())) {
            movingPlatforms1D.push_back(platform);
        }
    }

    // Check for collisions with each 2D moving platforms
    for (const MovingPlatform2D &platform: level.getMovingPlatforms2D()) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, platform.getBoundingBox())) {
            movingPlatforms2D.push_back(platform);
        }
    }

    // Check for collisions with each switching platforms
    for (const SwitchingPlatform &platform: level.getSwitchingPlatforms()) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, platform.getBoundingBox())) {
            switchingPlatforms.push_back(platform);
        }
    }

    // Check for collisions with size power-up
    for (const SizePowerUp &item: level.getSizePowerUp()) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, item.getBoundingBox())) {
            sizePowerUp.push_back(item);
        }
    }

    // Check for collisions with speed power-up
    for (const SpeedPowerUp &item: level.getSpeedPowerUp()) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, item.getBoundingBox())) {
            speedPowerUp.push_back(item);
        }
    }
}

void Game::narrowPhase() {
    // Check collisions with asteroids
    handleAsteroidsCollisions();

    // Handle collisions for all players
    for (Player &character: playerManager->getAlivePlayers()) {
        character.setCanMove(true);
        character.setIsOnPlatform(false);

        // Handle collisions according to player's mavity
        if (character.getMavity() > 0) handleCollisionsNormalMavity(character);
        else handleCollisionsReversedMavity(character);

        // Handle collisions with items
        handleCollisionsWithSizePowerUp(&character, &level, sizePowerUp);
        handleCollisionsWithSpeedPowerUp(&character, &level, speedPowerUp);

        handleCollisionsWithSaveZones(character, level, saveZones); // Handle collisions with save zones

        // Handle collisions with death zones and camera borders
        if (handleCollisionsWithCameraBorders(character.getBoundingBox(), camera.getBoundingBox())
            ||handleCollisionsWithDeathZones(character, deathZones))
        {
            playerManager->killPlayer(character);
        }
    }
}


/** COLLISION HANDLING **/

void Game::handleCollisionsNormalMavity(Player &player) const {
    // Check obstacles collisions only if the player has moved
    if (player.hasMoved()) {
        handleCollisionsWithObstacles(&player, obstacles);
    }

    handleCollisionsWithMovingPlatform1D(&player, movingPlatforms1D); // Handle collisions with 1D moving platforms
    handleCollisionsWithMovingPlatform2D(&player, movingPlatforms2D); // Handle collisions with 2D moving platforms
    handleCollisionsWithSwitchingPlatform(&player, switchingPlatforms); // Handle collisions with switching platforms
}

void Game::handleCollisionsReversedMavity(Player &player) const {
    // Check obstacles collisions only if the player has moved
    if (player.hasMoved()) {
        handleCollisionsSelcatsbOhtiw(&player, obstacles);
    }

    handleCollisionsD1mroftalPgnivoMhtiw(&player, movingPlatforms1D); // Handle collisions with 1D moving platforms
    handleCollisionsD2mroftalPgnivoMhtiw(&player, movingPlatforms2D); // Handle collisions with 2D moving platforms
    handleCollisionsMroftalPgnihctiwShtiw(&player, switchingPlatforms); // Handle collisions with switching platforms
}

void Game::handleAsteroidsCollisions() {
    std::vector<Asteroid> asteroids = level.getAsteroids();
    const std::vector<Polygon>& collisionObstacles = level.getZones(ZoneType::COLLISION);

    for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end();) {
        Asteroid& asteroid = *asteroidIt;
        bool alreadyExplode = false;
        std::vector<Player>& characters = playerManager->getAlivePlayers();

        // Check collisions with characters
        auto characterIt = characters.begin();
        while (!alreadyExplode && characterIt != characters.end()) {
            Player& character = *characterIt;
            if (checkAABBCollision(character.getBoundingBox(), asteroid.getBoundingBox())) {
                alreadyExplode = true;
                playerManager->killPlayer(character);
            }
            ++characterIt;
        }

        // Check collisions with obstacles
        auto obstacleIt = collisionObstacles.begin();
        while (!alreadyExplode && obstacleIt != collisionObstacles.end()) {
            const Polygon& obstacle = *obstacleIt;
            if (checkSATCollision(asteroid.getVertices(), obstacle)) {
                alreadyExplode = true;
                break; // No need to check further obstacles if asteroid already exploded
            }
            ++obstacleIt;
        }

        // Check if asteroid goes out of bounds
        if (!alreadyExplode && asteroid.getY() > camera.getY() + camera.getH() - asteroid.getH()) {
            alreadyExplode = true;
        }

        // Handle explosion or move to the next asteroid
        if (alreadyExplode) {
            asteroid.explode();
            asteroidIt = asteroids.erase(asteroidIt); // Remove the asteroid and update the iterator
        } else {
            ++asteroidIt; // Move to the next asteroid
        }
    }

    level.setAsteroids(asteroids);
}

/** END COLLISION HANDLING **/


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