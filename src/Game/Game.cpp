#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, int frameRate, std::vector<TTF_Font *> &fonts, const Camera &camera, Level level, bool *quitFlag)
        : window(window), renderer(renderer), frameRate(frameRate), fonts(fonts), camera(camera), level(std::move(level)), quitFlagPtr(quitFlag) {
}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

Camera *Game::getCamera() {
    return &camera;
}

Point Game::getAveragePlayersPositions() const {
    // Initialize sum variables
    float totalX = 0.0f;
    float totalY = 0.0f;

    // Calculate sum of x and y positions of all players
    for (const Player &character: characters) {
        totalX += character.getX();
        totalY += character.getY();
    }

    // Calculate the average by dividing by the total number of players
    size_t numPlayers = characters.size();
    if (numPlayers > 0) {
        float averageX = totalX / static_cast<float>(numPlayers);
        float averageY = totalY / static_cast<float>(numPlayers);
        return Point(averageX, averageY);
    } else {
        // If no players are present, return the origin point
        return Point(0.0f, 0.0f);
    }
}

std::vector<Player> &Game::getCharacters() {
    return characters;
}

Level &Game::getLevel() {
    return level;
}

int Game::getSaveSlot() const {
    return saveSlot;
}

int Game::getTickRate() const {
    return tickRate;
}


/** MODIFIERS **/

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name);
}

void Game::setRenderCameraPoint(bool state) {
    render_camera_point = state;
}

void Game::setRenderCameraArea(bool state) {
    render_camera_area = state;
}

void Game::setRenderPlayerColliders(bool state) {
    render_player_colliders = state;
}

void Game::setEnablePlatformsMovement(bool state) {
    enable_platforms_movement = state;
}

void Game::setSaveSlot(int slot) {
    saveSlot = slot;
    std::cout << "Game: Save slot set to " << slot << std::endl;
}

void Game::setFrameRate(int fps) {
    this->frameRate = fps;
}

void Game::toggleRenderTextures() {
    render_textures = !render_textures;
}

void Game::toggleRenderFps() {
    render_fps = !render_fps;
}


/** METHODS **/

void Game::initialize(int slot) {

    // If the player starts the server or is playing alone
    if (!Mediator::isClientRunning()) {
        saveSlot = slot;

        // Prepare the level
        using json = nlohmann::json;
        std::string slotFile = "saves/slot_" + std::to_string(saveSlot) + ".json";
        Point spawnPoint{};

        // If a save file exists in the slot, load the game from the save file
        if (std::filesystem::exists(slotFile)) {
            std::ifstream file(slotFile);
            json saveData;
            file >> saveData;

            level = Level(saveData["level"]);
            level.setLastCheckpoint(saveData["lastCheckpoint"]);
            spawnPoint = level.getSpawnPoints(saveData["lastCheckpoint"])[0];

            std::cout << "Game: Loaded save file from slot " << saveSlot << ", level: " << level.getMapName() << " at checkpoint " << level.getLastCheckpoint() << std::endl;
        }

        // If no save file exists in the slot, start a new game
        else {
            level = Level("diversity");
            spawnPoint = level.getSpawnPoints(0)[0];
            std::cout << "Game: No save file found in slot " << saveSlot << ", starting new game at level: " << level.getMapName() << std::endl;
        }

        // Add the initial player to the game
        Player initialPlayer(-1, spawnPoint, 48, 36);
        initialPlayer.setSpriteTextureByID(2);
        addCharacter(initialPlayer);
    }
}

Player *Game::findPlayerById(int id) {
    for (Player &character: characters) {
        if (character.getPlayerID() == id) {
            return &character;
        }
    }
    return nullptr;
}

int Game::findPlayerIndexById(int id) {
    for (size_t i = 0; i < characters.size(); ++i) {
        if (characters[i].getPlayerID() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Game::addCharacter(const Player &character) {
    characters.push_back(character);
}

void Game::removeCharacter(const Player *characterPtr) {
    // Check if the pointer is valid
    if (characterPtr == nullptr) {
        return; // Exit if the pointer is null
    }

    // Find the iterator corresponding to the character in the vector
    auto it = std::find_if(characters.begin(), characters.end(), [characterPtr](const Player &currentPlayer) {
        return &currentPlayer == characterPtr;
    });

    // Check if the character was found
    if (it != characters.end()) {
        // Erase the character from the vector
        characters.erase(it);
    }
}

void Game::handleKeyDownEvent(Player *player, const SDL_KeyboardEvent &keyEvent) {
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            // If the player is dead ignore
            if (player == nullptr) break;

            // If the coyote time is passed and the player is not already in a jump
            if (!player->getIsJumping()) {
                player->setWantToJump(true);
            }
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;
            player->setWantToMoveLeft(true);
            player->updateSprite(PLAYER_LEFT);
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;
            player->setWantToMoveRight(true);
            player->updateSprite(PLAYER_RIGHT);
            break;
        case SDL_SCANCODE_G:
            switchMavity(); // Switch mavity for all players
            break;
        case SDL_SCANCODE_M:
            if (level.getMapID() == 1) setLevel("assurance");
            else setLevel("diversity");
            break;
        case SDL_SCANCODE_ESCAPE:
            pause();
            break;
        case SDL_SCANCODE_DELETE:
            stop();
            break;
        case SDL_SCANCODE_LSHIFT:
            if (player == nullptr) break;
            player->setSprint(true);
            player->updateSprite(0);
            break;
        default:
            break;
    }
}

void Game::handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) {
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            if (player == nullptr) break;
            player->setWantToJump(false); // Disable jumping
            break;
        case SDL_SCANCODE_DOWN:
            if (player == nullptr) break;

            // Reset vertical movement if moving downwards
            if (player->getMoveY() > 0) player->setMoveY(0);
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;

            // Reset horizontal movement if moving left
            if (player->getMoveX() < 0) player->setMoveX(0);
            player->setWantToMoveLeft(false); // Disable left movement
            // Trigger idle animation if not moving right
            if (!player->getWantToMoveRight()) {
                player->updateSprite(0);
            } else {
                player->getSprite()->setFlipHorizontal(SDL_FLIP_NONE);
            }
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;

            // Reset horizontal movement if moving right
            if (player->getMoveX() > 0) {
                player->setMoveX(0);
            }
            player->setWantToMoveRight(false); // Disable right movement
            // Trigger idle animation if not moving left
            if (!player->getWantToMoveLeft()) {
                player->updateSprite(0);
            } else {
                player->getSprite()->setFlipHorizontal(SDL_FLIP_HORIZONTAL);
            }
            break;
        case SDL_SCANCODE_LSHIFT:
            if (player == nullptr) break;
            player->setSprint(false);
            player->updateSprite(0);
            break;
        default:
            break;
    }
}

void Game::handleEvents() {
    SDL_Event e;
    static uint16_t lastKeyboardStateMask = 0;
    Player *playerPtr = findPlayerById(-1);

    // Main loop handling every event one by one
    while (SDL_PollEvent(&e) != 0) {
        // Handle SDL_QUIT event
        if (e.type == SDL_QUIT) {
            std::cout << "Quit event detected\n" << std::endl;

            *quitFlagPtr = true;
            stop();
        }

        // Handle key events
        if (e.type == SDL_KEYUP) {
            handleKeyUpEvent(playerPtr, e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(playerPtr, e.key);
        }

        // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }

    // If the player is alive, send the keyboard state to the network after handling all events
    if (findPlayerById(-1) != nullptr) {
        sendKeyboardStateToNetwork(&lastKeyboardStateMask);
    }
}

void Game::calculatePlayersMovement(double deltaTime) {
    // Apply movement to all players
    for (Player &character: characters) {
        character.calculateMovement(deltaTime);
    }
}

void Game::applyPlayersMovement(double ratio) {
    // Apply movement for all players
    for (Player &character: characters) {
        character.applyMovement(ratio);
    }
}

void Game::killPlayer(const Player *player) {
    // Add the player to the dead characters list
    deadCharacters.push_back(*player);
    characters.erase(characters.begin() + findPlayerIndexById(player->getPlayerID()));
}

void Game::switchMavity() {
    // Change mavity for all player
    for (Player &character: characters) {
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
    for (const SizePowerUp &item: level.getItems<SizePowerUp>(ItemTypes::SIZE_POWER_UP)) {
        if (checkAABBCollision(broadPhaseAreaBoundingBox, item.getBoundingBox())) {
            sizePowerUp.push_back(item);
        }
    }
}

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

void Game::narrowPhase() {
    // Handle collisions for all players
    for (Player &character: characters) {
        character.setCanMove(true);
        character.setIsOnPlatform(false);

        handleCollisionsWithSaveZones(character, level, saveZones); // Handle collisions with save zones
        // Handle collisions with death zones
        if (handleCollisionsWithDeathZones(character, deathZones)) {
            killPlayer(&character);
        }
        handleCollisionsWithItems(&character, &level, sizePowerUp, ItemTypes::SIZE_POWER_UP);

        // Handle collisions according to player's mavity
        if (character.getMavity() > 0) handleCollisionsNormalMavity(character);
        else handleCollisionsReversedMavity(character);
    }
}

void Game::render() {
    Point cam = {camera.getX(), camera.getY()};

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render fps counter
    if (render_fps) {
        SDL_Color color = {160, 160, 160, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fonts[0], std::to_string(effectiveFrameFps).c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {10, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Render textures
    if (render_textures) {
        // Draw the characters
        for (Player &character: characters) {
            character.render(renderer, cam);
        }

        level.renderPolygonsDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
        level.renderItemsDebug(renderer, cam); // Draw the items
    }

    // Render collisions box
    else {
        // Draw the characters
        for (Player const &character: characters) {
            character.renderDebug(renderer, cam);
        }

        level.renderPolygonsDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
        level.renderItemsDebug(renderer, cam); // Draw the items
    }


    // DEBUG DRAWING OF APPLICATION CONSOLE :

    // Draw the camera point if enabled
    if (render_camera_point) camera.renderCameraPoint(renderer, getAveragePlayersPositions());

    // Draw the camera area if enabled
    if (render_camera_area) camera.renderCameraArea(renderer);

    // Draw the player's colliders if enabled
    if (render_player_colliders) {
        Player initialPlayer = *findPlayerById(-1);
        initialPlayer.renderColliders(renderer, cam);
    }

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
}

void Game::fixedUpdate() {
    handleEvents();
    calculatePlayersMovement(1.0 / tickRate);
}

void Game::update(double deltaTime, double ratio) {
    broadPhase();
    narrowPhase();
    if (enable_platforms_movement) level.applyPlatformsMovement(deltaTime);

    // Apply players movement directly with the calculated ratio
    applyPlayersMovement(ratio);

    camera.applyCameraMovement(getAveragePlayersPositions(), deltaTime);
    render();
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
    while (gameState == GameState::RUNNING) {
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

void Game::pause() {
    gameState = GameState::PAUSED;
}

void Game::stop() {
    gameState = GameState::STOPPED;
    characters.clear();
    saveSlot = -1;
}

void Game::saveGame() const {

    // Create a JSON object to store the game state
    using json = nlohmann::json;
    json gameStateJSON;

    // Store the game state

    // Save date to format YYYY-MM-DD (local time) using the C++ standard library
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = {};
    localtime_r(&currentTime, &localTime);

    std::stringstream ss;
    ss << std::put_time(&localTime, "%Y-%m-%d");

    gameStateJSON["date"] = ss.str();
    gameStateJSON["level"] = level.getMapName();
    gameStateJSON["lastCheckpoint"] = level.getLastCheckpoint();

    // Save the player state
    std::string saveFileName = "saves/slot_" + std::to_string(saveSlot) + ".json";

    // If the "saves" directory does not exist, create it
    if (!std::filesystem::exists(saveFileName)) {
        std::cout << "Game: Creating missing saves directory" << std::endl;
        std::filesystem::create_directory("saves");
    }

    // Open the file for writing
    std::ofstream file(saveFileName);

    // If the file was successfully created, write the game state to the file
    if (file.is_open()) {
        file << gameStateJSON.dump(4);
        file.close();
        std::cout << "Game: Saved game to slot " << saveSlot << std::endl;
    } else {
        std::cerr << "Game: Error saving game to slot " << saveSlot << std::endl;
    }
}


/** STATIC METHODS **/

void Game::sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMaskPtr) {

    // Get the current keyboard state mask
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    uint16_t currentKeyboardStateMask = Mediator::encodeKeyboardStateMask(keyboardState);

    // If the keyboard state has changed since the last message was sent
    if (currentKeyboardStateMask != *lastKeyboardStateMaskPtr) {
        // Send the new keyboard state mask to the server
        Mediator::sendPlayerUpdate(currentKeyboardStateMask);

        // Update the last keyboard state mask and the last send time
        *lastKeyboardStateMaskPtr = currentKeyboardStateMask;
    }
}

