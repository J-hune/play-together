#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Physics/Polygon.h"
#include "Camera.h"
#include "Objects/MovingPlatform1D.h"
#include "Objects/MovingPlatform2D.h"
#include "Objects/SwitchingPlatform.h"


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
     * @brief Returns the obstacles attribute.
     * @return A vector of Polygon.
     */
    [[nodiscard]] std::vector<Polygon> getObstacles() const;

    /**
     * @brief Returns the movingPlatform attribute.
     * @return A vector of MovingPlatform1D.
     */
    [[nodiscard]] std::vector<MovingPlatform1D> getMovingPlatforms1D() const;

    /**
     * @brief Returns the movingPlatform2D attribute.
     * @return A vector of MovingPlatform2D.
     */
    [[nodiscard]] std::vector<MovingPlatform2D> getMovingPlatforms2D() const;

    /**
     * @brief Returns the switchingPlatforms attribute.
     * @return A vector of SwitchingPlatform.
     */
    [[nodiscard]] std::vector<SwitchingPlatform> getSwitchingPlatforms() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Applies the movement of every platforms in the level.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void applyPlatformsMovement(float deltaTime);

    /**
     * @brief Renders the collisions by drawing obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderObstaclesDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the game by drawing the player and obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const;


private:
    /** ATTRIBUTES **/

    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */


    /** PRIVATE METHODS **/

    /**
     * @brief Loads obstacles from a map file.
     * @param mapName The name of the map file.
     */
    void loadPolygonsFromMap(const std::string &mapName);

};

#endif //PLAY_TOGETHER_LEVEL_H
