#include "../../include/Game/Game.h"

bool checkAABBCollision(const SDL_FRect &a, const SDL_FRect &b) {
    // Check for AABB collision
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Player &initialPlayer)
        : window(window), renderer(renderer), player(initialPlayer) {}

void Game::addCharacter(const Player &character) {
    characters.push_back(character);
}

void Game::teleportPlayer(float x, float y) {
    player.x = x;
    player.y = y;
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

void Game::loadPolygonsFromMap(const std::string& mapName) {
    obstacles.clear();

    // Define the file path for the polygon data
    std::string filePath = "../assets/maps/" + mapName + "/polygons.txt";
    std::cout << "File path: " << filePath << std::endl;

    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;

        // Read each line from the file
        while (std::getline(file, line)) {
            Polygon currentPolygon;
            std::istringstream iss(line);
            char dummy;

            // Extract points from each line
            while (iss >> dummy >> std::ws && dummy == '(') {
                int x;
                int y;
                iss >> x >> dummy >> y >> dummy;
                currentPolygon.vertices.emplace_back(x, y);
                iss >> dummy;
            }

            // Add the completed polygon to the obstacles vector
            obstacles.push_back(currentPolygon);
        }

        file.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
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

        // A key is pressed
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP :
                case SDLK_z:
                case SDLK_SPACE:
                    if (player.timeAfterFall > 0 && !player.isJumping){
                        player.wantToJump = true;
                    }
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    moveY = player.speed;
                    break;
                case SDLK_LEFT:
                case SDLK_q:
                    direction = PLAYER_LEFT;
                    player.wantToMove = true;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    direction = PLAYER_RIGHT;
                    player.wantToMove = true;
                    break;
                case SDLK_m:
                    printf("Loading map 'vow'\n");
                    loadPolygonsFromMap("vow");
                    break;
                default:
                    break;
            }
        }

        // A key is released
        else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_z:
                case SDLK_SPACE:
                    player.wantToJump = false;
                    break;
                case SDLK_LEFT:
                case SDLK_RIGHT:
                case SDLK_d:
                case SDLK_q:
                    player.wantToMove = false;
                    direction = 0;
                    break;
                default:
                    break;
            }
        }

        // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.x, (float)e.button.y + camera.y);
        }
    }
}

void Game::applyPlayerMovement(float &moveX, int direction, float  &timeSpeed, float &moveY) {

    // The player move on the x-axis
    if(player.wantToMove){
        timeSpeed += 0.1F;
        moveX = player.speedMax * player.speed * timeSpeed;
        //printf("\n%d\n\n\n\n", moveX > player.speedMax);
        if (moveX > player.speedMax){
            moveX = player.speedMax;
        }
        moveX = direction * moveX;
    }
    // The player doesn't move on the x-axis
    else{
        timeSpeed = 0;
        moveX = 0;
    }

    if(player.timeSpentJumping > PRESSURE_JUMP_MIN){
        player.minimumTimeJumpReached = true;
    }

    // The player press "jump button" and he doesn't maintain more than 6
    if(player.wantToJump && player.timeSpentJumping < PRESSURE_JUMP_MAX){
        player.minimumTimeJumpReached = false;
        // Player jump with the following mathematical function
        player.isJumping = true;
        moveY = -(float)(2 * player.timeSpentJumping - 0.3 * (player.timeSpentJumping * player.timeSpentJumping));
        player.timeSpentJumping += 0.1F;
    }
    else if(!player.minimumTimeJumpReached && player.timeSpentJumping < PRESSURE_JUMP_MIN){
        moveY = -(float)(2 * player.timeSpentJumping - 0.3 * (player.timeSpentJumping * player.timeSpentJumping));
        player.timeSpentJumping += 0.1F;
    }
    // The player doesn't jump
    else {
        player.timeSpentJumping = 0;
        player.wantToJump = false;

        // The player is on a platform
        if (player.isOnPlatform){
            moveY = 0;
            player.isJumping = false;
            player.timeAfterFall = COYOTE_TIME;
        }
        // The player is falling
        else{
            moveY = (player.timeSpentJumping <= 2) ? 2 - player.timeSpentJumping : 2;
            player.timeAfterFall -= 0.1F;
        }

    }

    handleCollisions(direction, moveY);
    if(player.canMove){
        player.x += moveX;
    }
    player.y += moveY;
}

void Game::getAveragePlayersPositions(float *x, float *y) const {
    float i = 1;  // Number of player in the game (at least one)

    // Initialization of the point on the initial player
    *x = player.x;
    *y = player.y;

    // Add x and y position of all players
    for (const Player &character : characters) {
        *x += character.x;
        *y += character.y;
        i++;
    }

    // Average x and y position of all players
    *x /= i; *y /= i;
}

void Game::initializeCameraPosition() {
    float x;
    float y;
    getAveragePlayersPositions(&x, &y);

    // Initialize the camera so that players are bottom left
    camera.x = x; camera.y = y - 2 * (camera.h / 3);

    // The point is on the right of the area
    if (x > camera.x + (camera.w - (camera.w / 2))) {
        camera.x += x - (camera.x + (camera.w - (camera.w / 2)));
    }
    // The point is on the left of the area
    else if (x < camera.x + (camera.w / 5)) {
        camera.x -= (camera.x + (camera.w / 5)) - x;
    }
    // The point is on the bottom of the area
    if (y > camera.y + (camera.h - (camera.h / 5))) {
        camera.y += y - (camera.y + (camera.h - (camera.h / 5)));
    }
    // The point is on the top of the area
    else if (y < camera.y + (camera.h / 5)) {
        camera.y -= (camera.y + (camera.h / 5)) - y;
    }
}

void Game::applyCameraMovement() {
    float x;
    float y;
    getAveragePlayersPositions(&x, &y);

    // The point is on the right of the area
    if (x > camera.x + camera.w - (camera.w / 2)) {
        camera.x += (x - (camera.x + (camera.w - (camera.w / 2)))) * LERP_SMOOTHING_FACTOR;
    }
    // The point is on the left of the area
    else if (x < camera.x + (camera.w / 5)) {
        camera.x -= ((camera.x + (camera.w / 5)) - x) * LERP_SMOOTHING_FACTOR;
    }
    // The point is on the bottom of the area
    if (y > camera.y + (camera.h - (camera.h / 5))) {
        camera.y += (y - (camera.y + (camera.h - (camera.h / 5)))) * LERP_SMOOTHING_FACTOR;
    }
    // The point is on the top of the area
    else if (y < camera.y + (camera.h / 5)) {
        camera.y -= ((camera.y + (camera.h / 5)) - y) * LERP_SMOOTHING_FACTOR;
    }
}

void Game::handleCollisions(int direction, float moveY) {
    player.canMove = true;
    if (moveY != 0 || direction != 0) {
        player.isOnPlatform = false;

        // Check for collisions with each obstacle
        for (const Polygon &obstacle: obstacles) {

            // If collision detected with the roof, the player can't jump anymore
            if (checkCollision(player.getVerticesRoof(), obstacle)) {
                printf("Collision detected on head\n");
                player.timeSpentJumping = PRESSURE_JUMP_MAX;
            }

            // If collision detected with the ground, the player is on a platform
            if (!player.isOnPlatform && checkCollision(player.getVerticesGround(), obstacle)) {
                printf("Collision detected on foot\n");
                player.isOnPlatform = true;
            }

            // If collision detected with the wall, the player can't move
            if (player.canMove && checkCollision(player.getVerticesHorizontal(direction), obstacle)) {
                printf("Collision detected for horizontal movement\n");
                player.canMove = false;
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

        // Check for collisions with camera borders
        if (player.x < camera.x || player.x > camera.x + camera.w - player.width ||
            player.y < camera.y || player.y > camera.y + camera.h - player.height) {

            printf("Collision detected with a camera border\n");
            player.x -= moveX;
            player.y -= moveY;
        }*/
    }
}

void Game::render() {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw the player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect playerRect = {player.x - camera.x, player.y - camera.y, player.width, player.height};
    SDL_RenderFillRectF(renderer, &playerRect);

    // Draw the characters
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const Player &character : characters) {
        SDL_FRect characterRect = {character.x - camera.x, character.y - camera.y, character.width, character.height};
        SDL_RenderFillRectF(renderer, &characterRect);
    }

    /*
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
    for (const Polygon &obstacle: obstacles) {
        for (size_t i = 0; i < obstacle.vertices.size(); ++i) {
            const auto &vertex1 = obstacle.vertices[i];
            const auto &vertex2 = obstacle.vertices[(i + 1) % obstacle.vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
    }

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
    SDL_Delay(4);
}

bool Game::checkCollision(const std::vector<Point> &playerVertices, const Polygon &obstacle) {
    // Check for convexity of the obstacle
    if (!isConvex(obstacle)) {
        printf("The obstacle is not convex\n");
        return false;
    }

    // Potential separation axes (normals to the sides of the rectangle)
    std::vector<Point> axes = {
            {1, 0},
            {0, 1}
    };

    // Add axes perpendicular to the sides of the polygon
    for (size_t i = 0; i < obstacle.vertices.size(); ++i) {
        const Point &vertex1 = obstacle.vertices[i];
        const Point &vertex2 = obstacle.vertices[(i + 1) % obstacle.vertices.size()];

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

        for (const Point &vertex: obstacle.vertices) {
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

bool Game::isConvex(const Polygon &polygon) {
    size_t n = polygon.vertices.size();
    if (n < 3) {
        return false;
    }

    // Check if the sum of interior angles equals (n - 2) * 180 degrees (convex polygon property) with a tolerance
    const double tolerance = 1e-3;
    return std::abs(polygon.totalAngles() - static_cast<double>(n - 2) * 180) < tolerance;
}

void Game::run() {
    int direction = 0;
    float moveX = 0;
    float moveY = 0;
    float timeSpeed = 0;

    // Game loop
    while (isRunning) {
        // Handle events, apply player movement, check collisions, apply camera movement, and render
        handleEvents(direction, moveY);
        applyPlayerMovement(moveX, direction, timeSpeed, moveY);
        applyCameraMovement();
        render();
    }

    // Close the window and clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::stop() {
    isRunning = false;
}