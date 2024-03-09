#include "../../include/Game/Level.h"

/**
 * @file Level.cpp
 * @brief Implements the Level class responsible for level object.
 */

/** CONSTRUCTOR **/

Level::Level(const std::string &map_name) {
    loadPolygonsFromMap(map_name);
    MovingPlatform1D platform(150, 0, 80, 20, 0.3F, 100, 400, false, false);
    movingPlatforms1D.push_back(platform);
}


/** ACCESSORS **/

std::vector<Polygon> Level::getObstacles() const {
    return obstacles;
}

std::vector<MovingPlatform1D> Level::getMovingPlatforms1D() const {
    return movingPlatforms1D;
}


/** METHODS **/

void Level::applyPlatformsMovement() {
    // Apply movement for 1D platforms
    for (MovingPlatform1D &platform: movingPlatforms1D) {
        platform.applyMovingPlatformMovement();
    }
}

void Level::renderPlatforms(SDL_Renderer *renderer, Camera camera) const {
    // Draw the moving platforms
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (const MovingPlatform1D &platform: movingPlatforms1D) {
        SDL_FRect platformRect = {platform.getX() - camera.getX(), platform.getY() - camera.getY(), platform.getW(), platform.getH()};
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