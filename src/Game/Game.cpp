#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, const Player &initialPlayer)
        : window(window), renderer(renderer), camera(camera), level(std::move(level)), player(initialPlayer) {}

Game::Game() :
            window(SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN)),
            renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
            level("experimentation"),
            player(-50, 50, 1, 2, 20, 30) {}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

Point Game::getAveragePlayersPositions() const {
    float i = 1;  // Number of player in the game (at least one)
    float x = player.getX(); // Initialization of the point on the initial player
    float y = player.getY();

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

void Game::setCameraIsShaking(bool state) {
    camera.setIsShaking(state);
}

void Game::setRenderPlayerColliders(bool state) {
    render_player_colliders = state;
}


/** FUNCTIONS **/

// Function to check AABB collision between two rectangles
bool checkAABBCollision(const SDL_FRect &a, const SDL_FRect &b) {
    // Check for AABB collision
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}


/** METHODS **/

void Game::teleportPlayer(float newX, float newY) {
    player.teleportPlayer(newX, newY);
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


void Game::handleKeyDownEvent(const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // If the coyote time is passed and the player is not already in a jump
            if (player.getTimeAfterFall() > 0 && !player.getIsJumping()) {
                player.setWantToJump(true);
            }
            break;
        case SDLK_LEFT:
        case SDLK_q:
            // If the player don't already move to the right
            if (!player.getWantToMoveRight()) {
                player.setDesiredDirection(PLAYER_LEFT);
                player.setWantToMoveLeft(true);
            }
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            // If the player don't already move to the left
            if (!player.getWantToMoveLeft()) {
                player.setDesiredDirection(PLAYER_RIGHT);
                player.setWantToMoveRight(true);
            }
            break;
        case SDLK_g:
            switchGravity = !switchGravity;
            player.setIsOnPlatform(false);
            player.setTimeSpentJumping(PRESSURE_JUMP_MAX);
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

void Game::handleKeyUpEvent(const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_z:
        case SDLK_SPACE:
            player.setWantToJump(false);
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            player.setWantToMoveRight(false);
            player.setFinishTheMovement(false);
            break;
        case SDLK_q:
        case SDLK_LEFT:
            player.setWantToMoveLeft(false);
            player.setFinishTheMovement(false);
            break;
        default:
            break;
    }
}

void Game::handleEvents() {
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
            handleKeyUpEvent(e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(e.key);
        }

            // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }
}


void Game::applyPlayerMovement() {
    // If the gravity is switched change the direction of the y-axis move
    if (switchGravity) {
        player.setMoveY(player.getMoveY()* -1);
    }

    // If the player can't move on x-axis, don't apply the movement
    if(!player.getCanMove()){
        player.setMoveX(0);
    }
    if(player.getX() < camera.getX()){
        player.setX(camera.getX());
    }
    else if(player.getX() + player.getW() > camera.getX() + camera.getW()) {
        player.setX(camera.getX() + camera.getW() - player.getW());
    }
    else{
        player.setX(player.getX() + player.getMoveX());
    }
    player.setY(player.getY() + player.getMoveY());
}

void Game::handleCollisions() {
    player.setCanMove(true);
    if (player.getMoveY() != 0 || player.getCurrentDirection() != 0) {
        player.setIsOnPlatform(false);

        // Check for collisions with each obstacle
        for (const Polygon &obstacle: level.getObstacles()) {

            // Normal gravity
            if (!switchGravity) {
                // If collision detected with the roof, the player can't jump anymore
                if (checkCollision(player.getVerticesRoof(), obstacle)) {
                    player.setTimeSpentJumping(PRESSURE_JUMP_MAX);
                }

                // If collision detected with the ground, the player is on a platform
                if (!player.getIsOnPlatform() && checkCollision(player.getVerticesGround(), obstacle)) {
                    player.setIsOnPlatform(true);
                }
            }
            // Reversed gravity
            else {
                // If collision detected with the roof, the player is on a platform
                if (checkCollision(player.getVerticesRoof(), obstacle)) {
                    player.setIsOnPlatform(true);
                }

                // If collision detected with the ground, the player can't jump anymore
                if (!player.getIsOnPlatform() && checkCollision(player.getVerticesGround(), obstacle)) {
                    player.setTimeSpentJumping(PRESSURE_JUMP_MAX);
                }
            }

            // If collision detected with the wall, the player can't move
            if (player.getCanMove() && checkCollision(player.getVerticesHorizontal(), obstacle)) {
                player.setCanMove(false);
                player.setTimeSpeed(0);
            }
        }

        /*
        // Check for collisions with other characters
        for (Player const &character : characters) {
            if (&character != &player && checkAABBCollision(player.getBoundingBox(), character.getBoundingBox())) {
                printf("Collision detected with another character\n");
                player.x -= moveX;
                player.y -= moveY;
            }
        }
        */


        /*// Check for collisions with down camera borders
         * if (player.y < camera.y){
         *
         * }
         */

        /*// Check for collisions with up camera borders
         * if (player.y > camera.y + camera.h - player.height){
         *
         * }
         */


        // Check for collisions with right camera borders
        if (player.getX() > camera.getX() + camera.getW() - player.getW()) {

            // Divide the velocity of the player
            player.setMoveX(player.getMoveX()/5);

            camera.setX(camera.getX() + player.getMoveX());

            // Check if others players touch the left camera borders
            for (Player &character : characters){
                if(character.getX() < camera.getX()){
                    character.setX(player.getMoveX());
                }
            }

        }
        // Check for collisions with left camera borders
        else if (player.getX() < camera.getX()) {

            // Divide the velocity of the player
            player.setMoveX(player.getMoveX()/5);

            camera.setX(camera.getX() + player.getMoveX());

            // Check if others players touch the right camera borders
            for (Player &character: characters) {
                if (character.getX() > camera.getX() + camera.getW() - character.getW()) {
                    character.setX(player.getMoveX());
                }
            }
        }
    }
}

void Game::render() {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw the player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect playerRect = {player.getX() - camera.getX(), player.getY() - camera.getY(), player.getW(), player.getH()};
    SDL_RenderFillRectF(renderer, &playerRect);

    // Draw the characters
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const Player &character : characters) {
        SDL_FRect characterRect = {character.getX() - camera.getX(), character.getY() - camera.getY(), character.getW(), character.getH()};
        SDL_RenderFillRectF(renderer, &characterRect);
    }


    // Draw the player's colliders
    if (render_player_colliders) {
        player.renderColliders(renderer, {camera.getX(), camera.getY()});
    }

    level.renderObstacles(renderer, camera); // Draw the obstacles

    // Draw the camera point
    if (render_camera_point) {
        camera.renderCameraPoint(renderer, getAveragePlayersPositions());
    }

    // Draw the camera area
    if (render_camera_area) {
        camera.renderCameraArea(renderer);
    }

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
    SDL_Delay(4);
}

bool Game::checkCollision(const std::vector<Point> &playerVertices, const Polygon &obstacle) {
    // Check for convexity of the obstacle
    if (!obstacle.isConvex()) {
        printf("The obstacle is not convex\n");
        return false;
    }

    // Potential separation axes (normals to the sides of the rectangle)
    std::vector<Point> axes = {
            {1, 0},
            {0, 1}
    };

    // Add axes perpendicular to the sides of the polygon
    for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
        std::vector<Point> vertices = obstacle.getVertices();
        const Point &vertex1 = vertices[i];
        const Point &vertex2 = vertices[(i + 1) % vertices.size()];

        Point edge = {vertex2.x - vertex1.x, vertex2.y - vertex1.y};
        Point axis = {-edge.y, edge.x};
        axes.push_back(axis);
    }

    for (Point axis: axes) {
        // Project the rectangle and the polygon onto the axis
        auto playerProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto playerProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: playerVertices) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            playerProjectionMin = std::min(playerProjectionMin, projection);
            playerProjectionMax = std::max(playerProjectionMax, projection);
        }

        auto obstacleProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto obstacleProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: obstacle.getVertices()) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            obstacleProjectionMin = std::min(obstacleProjectionMin, projection);
            obstacleProjectionMax = std::max(obstacleProjectionMax, projection);
        }

        // Check for separation on the axis
        if (playerProjectionMax < obstacleProjectionMin || obstacleProjectionMax < playerProjectionMin) {
            return false; // No collision detected
        }
    }

    return true; // Collision detected
}

void Game::run() {
    gameState = GameState::RUNNING;

    // Game loop
    while (gameState == GameState::RUNNING) {
        // Handle events, calculate player movement, check collisions, apply player movement, apply camera movement and render
        handleEvents();
        player.calculatePlayerMovement();
        handleCollisions();
        applyPlayerMovement();
        camera.applyCameraMovement(getAveragePlayersPositions());
        render();
    }
}

void Game::pause() {
    gameState = GameState::PAUSED;
}

void Game::stop() {
    gameState = GameState::STOPPED;

    // Reset the player position
    player.setX(50);
    player.setY(50);
}