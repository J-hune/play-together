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


void Game::handleKeyDownEvent(const SDL_KeyboardEvent& keyEvent, int &direction, float &moveY) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // If the coyote time is passed and the player is not already in a jump
            if (player.getTimeAfterFall() > 0 && !player.getIsJumping()) {
                player.setWantToJump(true);
            }
            break;
        case SDLK_DOWN:
        case SDLK_s:
            moveY = player.getSpeed();
            break;
        case SDLK_LEFT:
        case SDLK_q:
            // If the player don't already move to the right
            if (!player.getWantToMoveRight()) {
                direction = PLAYER_LEFT;
                player.setWantToMoveLeft(true);
            }
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            // If the player don't already move to the left
            if (!player.getWantToMoveLeft()) {
                direction = PLAYER_RIGHT;
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

void Game::handleEvents(int &direction, float &moveY) {
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
            handleKeyDownEvent(e.key, direction, moveY);
        }

            // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }
}


void Game::applyPlayerMovement(float &moveX, float &moveY) {
    // If the gravity is switched change the direction of the y-axis move
    if (switchGravity) {
        moveY *= -1;
    }

    // If the player can't move on x-axis, don't apply the movement
    if(!player.getCanMove()){
        moveX = 0;
    }
    if(player.getX() < camera.getX()){
        player.setX(camera.getX());
        printf("camera.x : %f\n", camera.getX());
    }
    else if(player.getX() + player.getW() > camera.getX() + camera.getW()) {
        player.setX(camera.getX() + camera.getW() - player.getW());
    }
    else{
        player.setX(player.getX() + moveX);
    }
    player.setY(player.getY() + moveY);
}

void Game::handleCollisions(int direction, float moveY, float &moveX) {
    player.setCanMove(true);
    if (moveY != 0 || direction != 0) {
        player.setIsOnPlatform(false);

        // Check for collisions with each obstacle
        for (const Polygon &obstacle: level.getObstacles()) {

            // Normal gravity
            if (!switchGravity) {
                // If collision detected with the roof, the player can't jump anymore
                if (checkCollision(player.getVerticesRoof(), obstacle)) {
                    printf("Collision detected on head\n");
                    player.setTimeSpentJumping(PRESSURE_JUMP_MAX);
                }

                // If collision detected with the ground, the player is on a platform
                if (!player.getIsOnPlatform() && checkCollision(player.getVerticesGround(), obstacle)) {
                    printf("Collision detected on foot\n");
                    player.setIsOnPlatform(true);
                }
            }
            // Reversed gravity
            else {
                // If collision detected with the roof, the player is on a platform
                if (checkCollision(player.getVerticesRoof(), obstacle)) {
                    printf("Collision detected on foot\n");
                    player.setIsOnPlatform(true);
                }

                // If collision detected with the ground, the player can't jump anymore
                if (!player.getIsOnPlatform() && checkCollision(player.getVerticesGround(), obstacle)) {
                    printf("Collision detected on head\n");
                    player.setTimeSpentJumping(PRESSURE_JUMP_MAX);
                }
            }

            // If collision detected with the wall, the player can't move
            if (player.getCanMove() && checkCollision(player.getVerticesHorizontal(direction), obstacle)) {
                printf("Collision detected for horizontal movement\n");
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
            moveX /= 5;

            camera.setX(camera.getX() + moveX);

            // Check if others players touch the left camera borders
            for (Player &character : characters){
                if(character.getX() < camera.getX()){
                    character.setX(moveX);
                }
            }

        }
        // Check for collisions with left camera borders
        else if (player.getX() < camera.getX()) {

            // Divide the velocity of the player
            moveX /= 5;

            camera.setX(camera.getX() + moveX);

            // Check if others players touch the right camera borders
            for (Player &character: characters) {
                if (character.getX() > camera.getX() + camera.getW() - character.getW()) {
                    character.setX(moveX);
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

    /* // Draw the player colliders
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    std::vector<Point> vertexRight = player.getVerticesHorizontal(1);
    for (size_t i = 0; i < vertexRight.size(); ++i) {
        const auto &vertex1 = vertexRight[i];
        const auto &vertex2 = vertexRight[(i + 1) % vertexRight.size()];
        SDL_RenderDrawLine(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    std::vector<Point> vertexLeft = player.getVerticesHorizontal(-1);
    for (size_t i = 0; i < vertexLeft.size(); ++i) {
        const auto &vertex1 = vertexLeft[i];
        const auto &vertex2 = vertexLeft[(i + 1) % vertexLeft.size()];
        SDL_RenderDrawLine(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertex = player.getVerticesRoof();
    for (size_t i = 0; i < vertex.size(); ++i) {
        const auto &vertex1 = vertex[i];
        const auto &vertex2 = vertex[(i + 1) % vertex.size()];
        SDL_RenderDrawLine(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertexGround = player.getVerticesGround();
    for (size_t i = 0; i < vertexGround.size(); ++i) {
        const auto &vertex1 = vertexGround[i];
        const auto &vertex2 = vertexGround[(i + 1) % vertexGround.size()];
        SDL_RenderDrawLine(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
    }
    */

    // Draw the obstacles
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const Polygon &obstacle: level.getObstacles()) {
        for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
            std::vector<Point> vertices = obstacle.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.getX(), vertex1.y - camera.getY(), vertex2.x - camera.getX(), vertex2.y - camera.getY());
        }
    }

    // Draw the camera point
    if (render_camera_point) {
        Point camera_point = getAveragePlayersPositions();
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_FRect cameraPointRect = {camera_point.x - camera.getX(), camera_point.y - camera.getY(), 20, 20};
        SDL_RenderFillRectF(renderer, &cameraPointRect);
        SDL_RenderDrawRectF(renderer, &cameraPointRect);
    }

    // Draw the camera area
    if (render_camera_area) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_FRect cameraRect = {camera.getArea().x, camera.getArea().y, camera.getArea().w + player.getW(), camera.getArea().h + player.getH() - 10};
        SDL_RenderDrawRectF(renderer, &cameraRect);
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
    int direction = 0;
    float moveX = 0;
    float moveY = 0;
    gameState = GameState::RUNNING;

    // Game loop
    while (gameState == GameState::RUNNING) {
        // Handle events, calculate player movement, check collisions, apply player movement, apply camera movement and render
        handleEvents(direction, moveY);
        player.calculatePlayerMovement(moveX, direction, moveY);
        handleCollisions(player.getCurrentDirection(), moveY, moveX);
        applyPlayerMovement(moveX, moveY);
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