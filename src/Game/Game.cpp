#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTOR **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, const Player &initialPlayer)
        : window(window), renderer(renderer), camera(camera), level(std::move(level)), player(initialPlayer) {}

Game ::Game() : level(level) {
        this->window = SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        this->player = Player (-50, 50, 2, 20, 30);
        this->level = std::move(Level("experimentation")); //needs to change with load
}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

Point Game::getAveragePlayersPositions() const {
    float i = 1;  // Number of player in the game (at least one)
    float x = player.x; // Initialization of the point on the initial player
    float y = player.y;

    // Add x and y position of all players
    for (const Player &character : characters) {
        x += character.x;
        y += character.y;
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

void Game::teleportPlayer(float x, float y) {
    player.x = x;
    player.y = y;
}

void Game::handleEvents(float &moveX, float &moveY) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        // Handle SDL_QUIT event
        if (e.type == SDL_QUIT) {
            printf("Quit event detected\n");
            stop();
            exit(0);
        }

        // Handle SDL_KEYDOWN and SDL_KEYUP events
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    moveY = -player.speed;
                    break;
                case SDLK_DOWN:
                    moveY = player.speed;
                    break;
                case SDLK_LEFT:
                    moveX = -player.speed;
                    break;
                case SDLK_RIGHT:
                    moveX = player.speed;
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
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_DOWN:
                    moveY = 0;
                    break;
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    moveX = 0;
                    break;
                default:
                    break;
            }
        }

        // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }
}

void Game::applyPlayerMovement(float moveX, float moveY) {
    player.x += moveX;
    player.y += moveY;
}



bool Game::checkCollision(const Player &player, const Polygon &obstacle) {
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
        auto player_projection_min = static_cast<float>(std::numeric_limits<int>::max());
        auto player_projection_max = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: player.getVertices()) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            player_projection_min = std::min(player_projection_min, projection);
            player_projection_max = std::max(player_projection_max, projection);
        }

        auto obstacle_projection_min = static_cast<float>(std::numeric_limits<int>::max());
        auto obstacle_projection_max = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: obstacle.getVertices()) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            obstacle_projection_min = std::min(obstacle_projection_min, projection);
            obstacle_projection_max = std::max(obstacle_projection_max, projection);
        }

        // Check for separation on the axis
        if (player_projection_max < obstacle_projection_min || obstacle_projection_max < player_projection_min) {
            return false; // No collision detected
        }
    }

    return true; // Collision detected
}

void Game::handleCollisions(float moveX, float moveY) {
    if (moveX != 0 || moveY != 0) {
        printf("Moving player to (%f, %f)\n", player.x, player.y);

        // Check for collisions with each obstacle
        for (const Polygon &obstacle: level.getObstacles()) {

            // If collision detected, revert player position
            if (checkCollision(player, obstacle)) {
                printf("Collision detected\n");
                player.x -= moveX;
                player.y -= moveY;
            }
        }

        // Check for collisions with other characters
        for (Player const &character: characters) {
            if (&character != &player && checkAABBCollision(player.getBoundingBox(), character.getBoundingBox())) {
                printf("Collision detected with another character\n");
                player.x -= moveX;
                player.y -= moveY;
            }
        }

        // Check for collisions with camera borders
        if (player.x < camera.getX() || player.x > camera.getX() + camera.getW() - player.width ||
            player.y < camera.getY() || player.y > camera.getY() + camera.getH() - player.height) {

            printf("Collision detected with a camera border\n");
            player.x -= moveX;
            player.y -= moveY;
        }
    }
}

void Game::render() {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw the player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect playerRect = {player.x - camera.getX(), player.y - camera.getY(), player.width, player.height};
    SDL_RenderFillRectF(renderer, &playerRect);

    // Draw the characters
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const Player &character : characters) {
        SDL_FRect characterRect = {character.x - camera.getX(), character.y - camera.getY(), character.width, character.height};
        SDL_RenderFillRectF(renderer, &characterRect);
    }

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
        SDL_FRect cameraRect = {camera.getArea().x, camera.getArea().y, camera.getArea().w + player.width, camera.getArea().h + player.height - 10};
        SDL_RenderDrawRectF(renderer, &cameraRect);
    }

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
}

void Game::run() {
    float moveX = 0;
    float moveY = 0;
    gameState = GameState::RUNNING;

    // Game loop
    while (gameState == GameState::RUNNING) {
        // Handle events, apply player movement, check collisions, apply camera movement, and render
        handleEvents(moveX, moveY);
        applyPlayerMovement(moveX, moveY);
        handleCollisions(moveX, moveY);
        camera.applyCameraMovement(getAveragePlayersPositions());
        render();

        SDL_Delay(4);
    }
}

void Game::pause() {
    gameState = GameState::PAUSED;
}

void Game::stop() {
    gameState = GameState::STOPPED;

    // Reset the player position
    player.x = 50;
    player.y = 50;
}