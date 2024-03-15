#include "../../include/Game/Level.h"

/**
 * @file Level.cpp
 * @brief Implements the Level class responsible for level object.
 */

/** CONSTRUCTOR **/

Level::Level(const std::string &map_name) {
    loadPolygonsFromMap(map_name);
}


/** ACCESSORS **/

std::vector<Polygon> Level::getObstacles() const {
    return obstacles;
}

std::vector<MovingPlatform1D> Level::getMovingPlatforms1D() const {
    return movingPlatforms1D;
}

std::vector<MovingPlatform2D> Level::getMovingPlatforms2D() const {
    return movingPlatforms2D;
}

std::vector<SwitchingPlatform> Level::getSwitchingPlatforms() const {
    return switchingPlatforms;
}


/** METHODS **/

void Level::renderObstaclesDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const Polygon &obstacle: obstacles) {
        for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
            std::vector<Point> vertices = obstacle.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
    }
}

void Level::applyPlatformsMovement() {
    // Apply movement for 1D platforms
    for (MovingPlatform1D &platform: movingPlatforms1D) {
        platform.applyMovement();
    }

    // Apply movement for 2D platforms
    for (MovingPlatform2D &platform: movingPlatforms2D) {
        platform.applyMovement();
    }

    // Apply movement for switching platforms
    for (SwitchingPlatform &platform: switchingPlatforms) {
        platform.applyMovement();
    }
}

void Level::renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const {
    // Draw the 1D moving platforms
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (const MovingPlatform1D &platform: movingPlatforms1D) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
    }

    // Draw the 2D moving platforms
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    for (const MovingPlatform2D &platform: movingPlatforms2D) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
    }

    // Draw the switching platforms
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    for (const SwitchingPlatform &platform: switchingPlatforms) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
    }
}

void Level::loadPolygonsFromMap(const std::string &mapName) {
    obstacles.clear();

    // Define the file path for the polygon data
    std::string filePath = std::string(MAPS_DIRECTORY) + mapName + "/" + POLYGONS_FILE;

    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;

        // Read each line from the file
        while (std::getline(file, line)) {
            std::vector<Point> vertices;
            std::istringstream iss(line);
            char dummy;

            // Extract points from each line
            while (iss >> dummy >> std::ws && dummy == '(') {
                int x;
                int y;
                iss >> x >> dummy >> y >> dummy;
                vertices.emplace_back(x, y);
                iss >> dummy;
            }
            // Add the completed polygon to the obstacles vector
            obstacles.emplace_back(vertices);
        }

        file.close();
        std::cout << "Loaded " << obstacles.size() << " polygons from the map " << mapName << "." << std::endl;
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}