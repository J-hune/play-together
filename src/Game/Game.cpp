#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, const Player &initialPlayer)
        : window(window), renderer(renderer), camera(camera), level(std::move(level)), initialPlayer(initialPlayer) {}

Game::Game() :
            window(SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN)),
            renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
            level("experimentation") {}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

Camera* Game::getCamera() {
    return &camera;
}

Point Game::getAveragePlayersPositions() const {
    float i = 1;  // Number of player in the game (at least one)
    float x = initialPlayer.getX(); // Initialization of the point on the initial player
    float y = initialPlayer.getY();

    // Add x and y position of all players
    for (const Player &character : characters) {
        x += character.getX();
        y += character.getY();
        i++;
    }

    // Average x and y position of all players
    x /= i; y /= i;

    return {x, y};
}


/** MODIFIERS **/

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name);
}

void Game::setRenderCameraPoint(bool state){
    render_camera_point = state;
}

void Game::setRenderCameraArea(bool state){
    render_camera_area = state;
}

void Game::setRenderPlayerColliders(bool state) {
    render_player_colliders = state;
}

void Game::setEnablePlatformsMovement(bool state) {
    enable_platforms_movement = state;
}

void Game::toggleRenderTextures() {
    render_textures = !render_textures;
}


/** FUNCTIONS **/

// Function to check AABB collision between two rectangles



/** METHODS **/

void Game::teleportPlayer(float newX, float newY) {
    initialPlayer.teleportPlayer(newX, newY);
}

void Game::addCharacter(const Player &character) {
    characters.push_back(character);
}

void Game::removeCharacter(const Player &character) {
    // Find the iterator corresponding to the character in the vector
    std::input_iterator auto it = std::ranges::find(characters.begin(), characters.end(), character);

    // Check if the character was found
    if (it != characters.end()) {
        // Erase the character from the vector
        characters.erase(it);
    }
}


void Game::handleKeyDownEvent(Player *player, const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // If the coyote time is passed and the player is not already in a jump
            if (!player->getIsJumping()) {
                player->setWantToJump(true);
            }
            break;
        case SDLK_LEFT:
        case SDLK_q:
            player->setWantToMoveLeft(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_HORIZONTAL);
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            player->setWantToMoveRight(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_NONE);
            break;
        case SDLK_g:
            switchMavity();
            player->setIsOnPlatform(false);
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
            player->getSprite()->toggleFlipVertical();
            break;
        case SDLK_m:
            printf("Loading map 'diversity'\n");
            setLevel("diversity");
            break;
        case SDLK_ESCAPE:
            pause();
            break;
        case SDLK_DELETE:
            stop();
            break;
        default:
            break;
    }
}

void Game::handleKeyUpEvent(Player *player, const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // Reset vertical movement if moving upwards
            if (player->getMoveY() < 0) player->setMoveY(0);
            player->setWantToJump(false); // Disable jumping
            break;
        case SDLK_DOWN:
            // Reset vertical movement if moving downwards
            if (player->getMoveY() > 0) player->setMoveY(0);
        case SDLK_q:
        case SDLK_LEFT:
            // Reset horizontal movement if moving left
            if (player->getMoveX() < 0) player->setMoveX(0);
            player->setWantToMoveLeft(false); // Disable left movement
            // Trigger idle animation if not moving right
            if (!player->getWantToMoveRight()) {
                player->getSprite()->setAnimation(Player::idle);
            }
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            // Reset horizontal movement if moving right
            if (player->getMoveX() > 0) {
                player->setMoveX(0);
            }
            player->setWantToMoveRight(false); // Disable right movement
            // Trigger idle animation if not moving left
            if (!player->getWantToMoveLeft()) {
                player->getSprite()->setAnimation(Player::idle);
            }
            break;
        default:
            break;
    }
}

void Game::handleEvents(Player *player) {
    SDL_Event e;

    // Main loop handling every event one by one
    while (SDL_PollEvent(&e) != 0) {
        // Handle SDL_QUIT event
        if (e.type == SDL_QUIT) {
            printf("Quit event detected\n");
            stop();
            exit(0);
        }

        // Handle key events
        if (e.type == SDL_KEYUP) {
            handleKeyUpEvent(player, e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(player, e.key);
        }

        // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }
}

void Game::calculatePlayersMovement() {
    initialPlayer.calculateMovement(deltaTime); // Calculate movement for the initial player

    // Apply movement for other players
    for (Player &character : characters) {
        character.calculateMovement(deltaTime);
    }
}

void Game::applyPlayersMovement() {
    initialPlayer.applyMovement(); // Apply movement for the initial player

    // Apply movement for other players
    for (Player &character : characters) {
        character.applyMovement();
    }
}

void Game::switchMavity() {
    initialPlayer.toggleMavity();
    for (Player &character : characters) {
        character.toggleMavity();
    }
}

void Game::handleCollisionsNormalMavity(Player &player) const {
    player.setCanMove(true);

    // Check obstacles collisions only if the player has moved
    if (player.getMoveY() != 0 || player.getDirectionX() != 0) {
        player.setIsOnPlatform(false);
        handleCollisionsWithObstacles(&player, level.getObstacles());
    }

    handleCollisionsWithMovingPlatform1D(&player, level.getMovingPlatforms1D()); // Handle collisions with 1D moving platforms
    handleCollisionsWithMovingPlatform2D(&player, level.getMovingPlatforms2D()); // Handle collisions with 2D moving platforms
    handleCollisionsWithSwitchingPlatform(&player, level.getSwitchingPlatforms()); // Handle collisions with switching platforms
}

void Game::handleCollisionsReversedMavity(Player &player) const {
    player.setCanMove(true);

    // Check obstacles collisions only if the player has moved
    if (player.getMoveY() != 0 || player.getDirectionX() != 0) {
        player.setIsOnPlatform(false);
        handleCollisionsSelcatsbOhtiw(&player, level.getObstacles());
    }

    handleCollisionsD1mroftalPgnivoMhtiw(&player, level.getMovingPlatforms1D()); // Handle collisions with 1D moving platforms
    handleCollisionsD2mroftalPgnivoMhtiw(&player, level.getMovingPlatforms2D()); // Handle collisions with 2D moving platforms
    handleCollisionsMroftalPgnihctiwShtiw(&player, level.getSwitchingPlatforms()); // Handle collisions with switching platforms
}

void Game::handleCollisions() {
    // Handle collisions for the initial player
    if (initialPlayer.getMavity() > 0) handleCollisionsNormalMavity(initialPlayer);
    else handleCollisionsReversedMavity(initialPlayer);

    // Handle collisions for other players
    for (Player &character : characters) {
        if (initialPlayer.getMavity() > 0) handleCollisionsNormalMavity(character);
        else handleCollisionsReversedMavity(character);
    }
}

void Game::render() {
    Point cam = {camera.getX(), camera.getY()};

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render textures
    if (render_textures) {
        initialPlayer.renderDebug(renderer, cam); // Draw the initial player

        // Draw the characters
        for (Player &character: characters) {
            character.render(renderer, cam);
        }

        level.renderObstaclesDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }
    // Render collisions box
    else {
        initialPlayer.renderDebug(renderer, cam); // Draw the initial player

        // Draw the characters
        for (Player const &character: characters) {
            character.renderDebug(renderer, cam);
        }

        level.renderObstaclesDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }

    // DEBUG DRAWING OF APPLICATION CONSOLE :

    // Draw the camera point if enabled
    if (render_camera_point) camera.renderCameraPoint(renderer, getAveragePlayersPositions());

    // Draw the camera area if enabled
    if (render_camera_area) camera.renderCameraArea(renderer);

    // Draw the player's colliders if enabled
    if (render_player_colliders) initialPlayer.renderColliders(renderer, cam);

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
}

void Game::run() {
    gameState = GameState::RUNNING;
    Uint32 lastTime = SDL_GetTicks();

    // Game loop
    while (gameState == GameState::RUNNING) {
        Uint32 start = SDL_GetTicks(); // Get time before execution

        // MAIN EVENTS
        handleEvents(&initialPlayer);
        for (Player &character : characters) handleEvents(&character);
        if (enable_platforms_movement) level.applyPlatformsMovement(deltaTime);
        calculatePlayersMovement();
        handleCollisions();
        applyPlayersMovement();
        camera.applyCameraMovement(getAveragePlayersPositions(), deltaTime);
        render();

        // FRAME RATE LIMITATION
        Uint32 end = SDL_GetTicks(); // Get time after execution
        deltaTime = (float)(end - lastTime) / 1000; // Get delta time
        lastTime = end; // Last time delta time was calculated
        Uint32 elapsedTime = end - start; // Get execution time

        // If execution time is less than a frame, wait until a frame has passed
        if (1000 / fps > elapsedTime) {
            SDL_Delay(1000 / fps - elapsedTime);
        }
    }
}

void Game::pause() {
    gameState = GameState::PAUSED;
}

void Game::stop() {
    gameState = GameState::STOPPED;

    // Reset the player position
    initialPlayer.setX(50);
    initialPlayer.setY(50);
}