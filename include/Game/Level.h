#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Polygon.h"
#include "Camera.h"

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
    /** CONSTRUCTOR **/

    Level(const std::string &map_name);


    /** ACCESSORS **/

    /**
     * @brief Return the obstacles attribute.
     * @return A vector of Polygon.
     */
    [[nodiscard]] std::vector<Polygon> getObstacles() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Renders the collisions by drawing obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderObstacles(SDL_Renderer *renderer, Camera camera) const;


private:
    /** ATTRIBUTES **/

    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */


    /** PRIVATE METHODS **/

    /**
     * @brief Loads obstacles from a map file.
     * @param mapName The name of the map file.
     */
    void loadPolygonsFromMap(const std::string &mapName);

};

#endif //PLAY_TOGETHER_LEVEL_H
