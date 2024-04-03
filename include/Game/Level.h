#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Physics/Polygon.h"
#include "Camera.h"
#include "Platforms/MovingPlatform1D.h"
#include "Platforms/MovingPlatform2D.h"
#include "Platforms/SwitchingPlatform.h"
#include "Items/SizePowerUp.h"
#include "../../dependencies/json.hpp"

// Define constants for directories and file names
constexpr char MAPS_DIRECTORY[] = "assets/maps/";

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

    explicit Level(const std::string &map_name);


    /** ACCESSORS **/

    /**
     * @brief Return the ID of the map.
     * @return An integer representing the ID of the map.
     */
    [[nodiscard]] int getMapID() const;

    /**
     * @brief Return the name of the map.
     * @return A string representing the name of the map.
     */
    [[nodiscard]] std::string getMapName() const;

    /**
     * @brief Return the spawn points of the map.
     * @return A vector of Point representing the spawn points of the map.
     */
    [[nodiscard]] std::array<Point, 4> getSpawnPoints(int index) const;

    /**
     * @brief Return the zones of a specific type.
     * @param type Represents the type of zone.
     * @return A vector of Polygon.
     */
    [[nodiscard]] std::vector<Polygon> getZones(ZoneType type) const;

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

    /**
     * @brief Return the switchingPlatforms attribute.
     * @return A vector of SwitchingPlatform.
     */
    [[nodiscard]] std::vector<SwitchingPlatform> getSwitchingPlatforms() const;

    /**
    * @brief Return an item attribute of generic type.
    * @tparam T The type of items to return.
    * @param type The type of items to retrieve.
    * @return The item attribute of generic type.
    */
    template<typename T>
    [[nodiscard]] std::vector<T> getItems(ItemTypes type) const {
        switch(type) {
            using enum ItemTypes;
            case SIZE_POWER_UP: return sizePowerUp;
            default: return {};
        }
    }

    /**
     * @brief Return the last checkpoint reached by the player.
     * @return A short representing the last checkpoint reached by the player.
     */
    [[nodiscard]] short getLastCheckpoint() const;


    /** MODIFIERS **/

    /**
     * @brief Set the last checkpoint reached by the player.
     * @param checkpoint Represents the last checkpoint reached by the player.
     */
    void setLastCheckpoint(short checkpoint);

    /**
    * @brief Set an item attribute of generic type.
    * @tparam T The type of item to set.
    * @param type The type of item to modify.
    */
    template<typename T>
    void setItem(std::vector<T> item, ItemTypes type) {
        using enum ItemTypes;
        if (type == SIZE_POWER_UP) sizePowerUp = item;
    }


    /** PUBLIC METHODS **/

    /**
     * @brief Applies the movement of every platforms in the level.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void applyPlatformsMovement(double deltaTime);

    /**
     * @brief Renders the collisions by drawing obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderPolygonsDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the game by drawing the player and obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the items by drawing rectangles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderItemsDebug(SDL_Renderer *renderer, Point camera) const;

private:
    /** ATTRIBUTES **/

    int mapID = 0; /**< Represents the ID of the map. */
    std::string mapName; /**< Represents the name of the map. */
    std::vector<std::array<Point, 4>> spawnPoints; /**< Represents the spawn points of the map. */
    std::vector<Polygon> collisionZones; /**< Collection of polygons representing obstacles. */
    std::vector<Polygon> iceZones; /**< Collection of polygons representing ice zones. */
    std::vector<Polygon> sandZones; /**< Collection of polygons representing sand zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> cinematicZones; /**< Collection of polygons representing cinematic zones. */
    std::vector<Polygon> bossZones; /**< Collection of polygons representing boss zones. */
    std::vector<Polygon> eventZones; /**< Collection of polygons representing event zones. */
    std::vector<Polygon> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */
    std::vector<SizePowerUp> sizePowerUp; /**< Collection of SizePowerUp representing size power-up. */
    short lastCheckpoint = 0; /**< Represents the last checkpoint reached by the player. */

    /** PRIVATE METHODS **/

    /**
     * @brief Load the properties of the map.
     * @param mapName Represents the name of the map file.
     */
    void loadMapProperties(const std::string &mapName);

    /**
     * @brief Load the polygons from a JSON file.
     * @param jsonData Represents the JSON data.
     * @param zoneName Represents the name of the zone.
     * @param[out] zones Represents the collection of polygons.
     * @return The number of polygons loaded.
     */
    static int loadPolygonsFromJson(const nlohmann::json &jsonData, const std::string &zoneName, std::vector<Polygon> &zones, ZoneType type);

    /**
     * @brief Load the polygons from a map.
     * @param mapName Represents the name of the map.
     * @see loadPolygonsFromJson() for sub-function.
     */
    void loadPolygonsFromMap(const std::string &mapName);

    /**
     * @brief Load the platforms from a map. (1D, 2D and switching platforms)
     * @param mapName Represents the name of the map.
     */
    void loadPlatformsFromMap(const std::string &mapName);

    /**
     * @brief Load the items from a map.
     * @param mapName Represents the name of the map.
     */
    void loadItemsFromMap(const std::string &mapName);


};

#endif //PLAY_TOGETHER_LEVEL_H
