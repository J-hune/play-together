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

void Level::loadPolygonsFromMap(const std::string &mapName) {
    obstacles.clear();

    // Define the file path for the polygon data
    std::string filePath = std::string(MAPS_DIRECTORY) + mapName + "/" + POLYGONS_FILE;

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
        std::cout << "Loaded " << obstacles.size() << " polygons from the map " << mapName << "." << std::endl;
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}