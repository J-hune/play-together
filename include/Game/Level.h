#ifndef PLAY_TOGETHER_LEVEL_H
#define PLAY_TOGETHER_LEVEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Physics/Polygon.h"
#include "../Physics/AABB.h"
#include "../Sounds/Music.h"
#include "Camera.h"
#include "Events/Asteroid.h"
#include "Platforms/MovingPlatform1D.h"
#include "Platforms/MovingPlatform2D.h"
#include "Platforms/SwitchingPlatform.h"
#include "Items/SizePowerUp.h"
#include "Items/SpeedPowerUp.h"
#include "../Utils/Mediator.h"
#include "../../dependencies/json.hpp"
#include "Events/Rescue.h"

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
    Level() = default;

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
     * @brief Return a music from the musics attribute.
     * @param id The id of the music to retrieve.
     * @return A Music object.
     */
    [[nodiscard]] Music& getMusicById(int id) ;

    /**
     * @brief Return the zones of a specific type.
     * @param type Represents the type of zone.
     * @return A vector of Polygon.
     */
    [[nodiscard]] std::vector<Polygon> getZones(PolygonType type) const;

    /**
     * @brief Return the zones of a specific type.
     * @param type Represents the type of zone.
     * @return A vector of AABB.
     */
    [[nodiscard]] std::vector<AABB> getZones(AABBType type) const;

    /**
     * @brief Return the asteroids attribute.
     * @return A vector of Asteroid.
     */
    [[nodiscard]] std::vector<Asteroid> getAsteroids() const;

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
     * @brief Return the sizePowerUp attribute.
     * @return A vector of SizePowerUp.
     */
    [[nodiscard]] std::vector<SizePowerUp> getSizePowerUp() const;

    /**
     * @brief Return the speedPowerUp attribute.
     * @return A vector of SpeedPowerUp.
     */
    [[nodiscard]] std::vector<SpeedPowerUp> getSpeedPowerUp() const;

    /**
     * @brief Return the last checkpoint reached by the player.
     * @return A short representing the last checkpoint reached by the player.
     */
    [[nodiscard]] short getLastCheckpoint() const;

    [[nodiscard]] std::vector<Rescue> getRescue();


    /** MUTATORS **/

    /**
     * @brief Set the last checkpoint reached by the player.
     * @param checkpoint Represents the last checkpoint reached by the player.
     */
    void setLastCheckpoint(short checkpoint);

    /**
     * @brief Set the asteroids attribute.
     * @param value Represents the asteroids attribute.
     */
    void setAsteroids(const std::vector<Asteroid> &value);

    /**
     * @brief Remove an item from sizePowerUp attribute.
     * @param item The item to remove.
     */
    void removeItemFromSizePowerUp(SizePowerUp const &item);

    /**
     * @brief Remove an item from speedPowerUp attribute.
     * @param item The item to remove.
     */
    void removeItemFromSpeedPowerUp(SpeedPowerUp const &item);


    /** PUBLIC METHODS **/

    /**
     * @brief Applies the movement to all asteroid in the game.
     * @param delta_time The time elapsed since the last frame in seconds.
     * @see applyMovement() for applying movement to one asteroid.
     */
    void applyAsteroidsMovement(double delta_time);

    /**
     * @brief Applies the movement of every platforms in the level.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void applyPlatformsMovement(double delta_time);

    /**
     * @brief Generates a specified number of asteroids in the game.
     * @param nbAsteroid The number of asteroids must have in the game.
     */
    void generateAsteroid(int nbAsteroid, Point camera, size_t seed);

    /**
     * @brief Add an asteroid received from network to the game.
     * @param asteroid The asteroid to add.
     */
    void addAsteroid(Asteroid const &asteroid);

    /**
     * @brief Renders the collisions by drawing obstacles.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderPolygonsDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders asteroids by drawing sprites.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderAsteroids(SDL_Renderer *renderer, Point camera);

    /**
     * @brief Renders asteroids by drawing collisions boxes.
     * @param renderer Represents the renderer of the game.
     * @param camera Represents the camera of the game.
     */
    void renderAsteroidsDebug(SDL_Renderer *renderer, Point camera) const;

    /**
     * @brief Renders the platforms by drawing collisions boxes.
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
    std::vector<Music> musics; /**< Represents the musics of the map. */

    std::vector<Polygon> collisionZones; /**< Collection of polygons representing obstacles. */
    std::vector<Polygon> iceZones; /**< Collection of polygons representing ice zones. */
    std::vector<Polygon> sandZones; /**< Collection of polygons representing sand zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> cinematicZones; /**< Collection of polygons representing cinematic zones. */
    std::vector<Polygon> bossZones; /**< Collection of polygons representing boss zones. */
    std::vector<Polygon> eventZones; /**< Collection of polygons representing event zones. */
    std::vector<AABB> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<AABB> toggleGravityZones; /**< Collection of polygons representing toggle gravity zones. */
    std::vector<AABB> increaseFallSpeedZones; /**< Collection of polygons representing increase fall speed zones. */
    std::vector<Asteroid> asteroids; /**< Collection of Asteroid representing asteroids. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */
    std::vector<SizePowerUp> sizePowerUp; /**< Collection of SizePowerUp representing size power-up. */
    std::vector<SpeedPowerUp> speedPowerUp; /**< Collection of SpeedPowerUp representing speed power-up. */
    short lastCheckpoint = 0; /**< Represents the last checkpoint reached by the player. */
    std::vector<Rescue> rescueZones;/**Collection of areas where the players can come back*/

    /** PRIVATE METHODS **/

    /**
     * @brief Load the properties of the map.
     * @param map_file_name Represents the name of the map file.
     */
    void loadMapProperties(const std::string &map_file_name);

    /**
     * @brief Load the polygons from a JSON file.
     * @param json_data Represents the JSON data.
     * @param zone_name Represents the name of the zone.
     * @param[out] zones Represents the collection of polygons.
     * @return The number of polygons loaded.
     */
    static int loadPolygonsFromJson(const nlohmann::json &json_data, const std::string &zone_name, std::vector<Polygon> &zones, PolygonType type);

    /**
     * @brief Load the AABB from a JSON file.
     * @param json_data Represents the JSON data.
     * @param zone_name Represents the name of the zone.
     * @param[out] zones Represents the collection of AABB.
     * @return The number of AABB loaded.
     */
    static int loadAABBFromJson(const nlohmann::json &json_data, const std::string &zone_name, std::vector<AABB> &zones, AABBType type);

    /**
     * @brief Load the polygons from a map.
     * @param map_file_name Represents the name of the map.
     * @see loadPolygonsFromJson() for sub-function.
     */
    void loadPolygonsFromMap(const std::string &map_file_name);

    /**
     * @brief Load the platforms from a map. (1D, 2D and switching platforms)
     * @param map_file_name Represents the name of the map.
     */
    void loadPlatformsFromMap(const std::string &map_file_name);

    /**
     * @brief Load the items from a map.
     * @param map_file_name Represents the name of the map.
     */
    void loadItemsFromMap(const std::string &map_file_name);

    void loadRescueZonesFormMap(const std::string &map_file_name);
};

#endif //PLAY_TOGETHER_LEVEL_H
