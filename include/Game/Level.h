#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Polygon.h"

// Define constants for directories and file names
constexpr char MAPS_DIRECTORY[] = "../assets/maps/";
constexpr char POLYGONS_FILE[] = "polygons.txt";


/**
 * @file Level.h
 * @brief Defines the Level class responsible for level object.
 */

/**
 * @class Level
 * @brief Represents the level object including obstacles.
 */
class Level {
public:
    Level(const std::string &map_name); /**< Constructor for the Level class. */


    /** ACCESSORS **/

    /**
     * @brief Return the obstacles attribute.
     * @return A vector of Polygon.
     */
    [[nodiscard]] std::vector<Polygon> getObstacles() const;


private:
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */

    /**
     * @brief Loads obstacles from a map file.
     * @param mapName The name of the map file.
     */
    void loadPolygonsFromMap(const std::string &mapName);


};


#endif //PLAY_TOGETHER_LEVEL_H
