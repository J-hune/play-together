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

std::vector<Polygon> Level::getObstacles() const{
    return obstacles;
}


/** METHODS **/

void Level::renderObstacles(SDL_Renderer *renderer, Camera camera) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const Polygon &obstacle: obstacles) {
        for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
            std::vector<Point> vertices = obstacle.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.getX(), vertex1.y - camera.getY(), vertex2.x - camera.getX(), vertex2.y - camera.getY());
        }
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