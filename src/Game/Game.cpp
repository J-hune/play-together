#include <iostream>
#include <fstream>
#include <sstream>
#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Player &initialPlayer)
        : window(window), renderer(renderer), player(initialPlayer) {}

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

void Game::handleEvents(int &moveX, int &moveY) {
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
                case SDLK_m:
                    printf("Loading map 'vow'\n");
                    loadPolygonsFromMap("vow");
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
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                printf("Mouse clicked at (%d, %d)\n", e.button.x, e.button.y);
            }
        }
    }
}

void Game::applyPlayerMovement(int moveX, int moveY) {
    player.x += moveX;
    player.y += moveY;
}

void Game::handleCollisions(int moveX, int moveY) {
    if (moveX != 0 || moveY != 0) {
        printf("Moving player to (%d, %d)\n", player.x, player.y);

        // Check for collisions with each obstacle
        for (const Polygon &obstacle: obstacles) {

            // If collision detected, revert player position
            if (checkCollision(player, obstacle)) {
                printf("Collision detected\n");
                player.x -= moveX;
                player.y -= moveY;
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
    SDL_Rect playerRect = {player.x, player.y, player.width, player.height};
    SDL_RenderFillRect(renderer, &playerRect);

    // Draw the obstacles
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const Polygon &obstacle: obstacles) {
        for (size_t i = 0; i < obstacle.vertices.size(); ++i) {
            const auto &vertex1 = obstacle.vertices[i];
            const auto &vertex2 = obstacle.vertices[(i + 1) % obstacle.vertices.size()];
            SDL_RenderDrawLine(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
        }
    }

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
    SDL_Delay(4);
}

bool Game::checkCollision(const Player &player, const Polygon &obstacle) {
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
        int playerProjectionMin = INT_MAX;
        int playerProjectionMax = INT_MIN;

        for (const Point &vertex: player.getVertices()) {
            int projection = vertex.x * axis.x + vertex.y * axis.y;
            playerProjectionMin = std::min(playerProjectionMin, projection);
            playerProjectionMax = std::max(playerProjectionMax, projection);
        }

        int obstacleProjectionMin = INT_MAX;
        int obstacleProjectionMax = INT_MIN;

        for (const Point &vertex: obstacle.vertices) {
            int projection = vertex.x * axis.x + vertex.y * axis.y;
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
    const double tolerance = 1e-6;
    return fabs(polygon.totalAngles() - static_cast<double>((n - 2) * 180)) < tolerance;
}

void Game::run() {
    int moveX = 0;
    int moveY = 0;

    // Game loop
    while (isRunning) {
        // Handle events, apply player movement, check collisions, and render
        handleEvents(moveX, moveY);
        applyPlayerMovement(moveX, moveY);
        handleCollisions(moveX, moveY);
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