#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Polygon.h"
#include "Camera.h"
#include "Objects/MovingPlatform1D.h"
#include "Objects/MovingPlatform2D.h"


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

    /**
     * @brief Return the movingPlatform attribute.
     * @return A vector of MovingPlatform1D.
     */
    [[nodiscard]] std::vector<MovingPlatform1D> getMovingPlatforms1D() const;

    /**
     * @brief Return the movingPlatform2D attribute.
     * @return A vector of MovingPlatform2D.
     */
    [[nodiscard]] std::vector<MovingPlatform2D> getMovingPlatforms2D() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Apply the movement of every platforms in the level.
     */
    void applyPlatformsMovement();

    /**
     * @brief Renders the game by drawing the player and obstacles.
     * @param[out] renderer The renderer associated to the current game
     * @param camera The camera associated to the current game
     */
    void renderPlatforms(SDL_Renderer *renderer, Point camera) const;


private:
    /** ATTRIBUTES **/

    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */


    /** PRIVATE METHODS **/

    /**
     * @brief Loads obstacles from a map file.
     * @param mapName The name of the map file.
     */
    void loadPolygonsFromMap(const std::string &mapName);

};

#endif //PLAY_TOGETHER_LEVEL_H
