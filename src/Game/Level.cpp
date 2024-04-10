#include "../../include/Game/Level.h"

/**
 * @file Level.cpp
 * @brief Implements the Level class responsible for level object.
 */

/** CONSTRUCTOR **/

Level::Level(const std::string &map_name) {
    std::cout << "Level: Loading level " << map_name << "..." << std::endl;

    loadMapProperties(map_name);
    loadPolygonsFromMap(map_name);
    loadPlatformsFromMap(map_name);
    loadItemsFromMap(map_name);
}


/** ACCESSORS **/

int Level::getMapID() const {
    return mapID;
}

std::string Level::getMapName() const {
    return mapName;
}

std::array<Point, 4> Level::getSpawnPoints(int index) const {
    return spawnPoints[index];
}

std::vector<Polygon> Level::getZones(PolygonType type) const {
    switch(type) {
        using enum PolygonType;
        case COLLISION: return collisionZones;
        case ICE: return iceZones;
        case SAND: return sandZones;
        case DEATH: return deathZones;
        case CINEMATIC: return cinematicZones;
        case BOSS: return bossZones;
        case EVENT: return eventZones;
        default: return {};
    }
}

std::vector<AABB> Level::getZones(AABBType type) const {
    switch(type) {
        using enum AABBType;
        case SAVE: return saveZones;
        case TOGGLE_GRAVITY: return toggleGravityZones;
        case INCREASE_FALL_SPEED: return increaseFallSpeedZones;
        default: return {};
    }
}

Music& Level::getMusicById(int id) {
    return musics[id];
}

std::vector<Asteroid> Level::getAsteroids() const {
    return asteroids;
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

std::vector<SizePowerUp> Level::getSizePowerUp() const {
    return sizePowerUp;
}

std::vector<SpeedPowerUp> Level::getSpeedPowerUp() const {
    return speedPowerUp;
}

short Level::getLastCheckpoint() const {
    return lastCheckpoint;
}


/** MUTATORS **/

void Level::setLastCheckpoint(short checkpoint) {
    lastCheckpoint = checkpoint;
}

void Level::setAsteroids(std::vector<Asteroid> const &value) {
    this->asteroids = value;
}

void Level::removeItemFromSizePowerUp(SizePowerUp const &item) {
    // Search the item and remove it
    auto it = std::ranges::find(sizePowerUp, item);
    if (it != sizePowerUp.end()) {
        sizePowerUp.erase(it);
    }
}

void Level::removeItemFromSpeedPowerUp(SpeedPowerUp const &item) {
    // Search the item and remove it
    auto it = std::ranges::find(speedPowerUp, item);
    if (it != speedPowerUp.end()) {
        speedPowerUp.erase(it);
    }
}


/** METHODS **/

void Level::generateAsteroid(int nbAsteroid, Point camera, size_t seed) {
    // Loop to generate asteroids until the desired number is reached
    for (auto i = static_cast<int>(asteroids.size()); i < nbAsteroid; i++){
        // Add a new asteroid to the asteroids vector with coordinates based on the camera position
        Asteroid new_asteroid(camera.x, camera.y, seed);
        asteroids.emplace_back(new_asteroid);

        // Send the asteroid throw the network
        if (Mediator::isServerRunning()) {
            Mediator::sendAsteroidCreation(new_asteroid);
        }
    }
}

void Level::addAsteroid(Asteroid const &asteroid) {
    asteroids.emplace_back(asteroid);
}

void Level::renderPolygonsDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (const Polygon &obstacle: collisionZones) {
        for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
            std::vector<Point> vertices = obstacle.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 25, 25, 255);
    for (const Polygon &obstacle: deathZones) {
        for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
            std::vector<Point> vertices = obstacle.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
    for (const AABB &save_zone: saveZones) {
        // Draw only the outline of the save zone
        SDL_FRect save_zone_rect = {save_zone.getX() - camera.x, save_zone.getY() - camera.y, save_zone.getWidth(), save_zone.getHeight()};
        SDL_RenderDrawRectF(renderer, &save_zone_rect);
    }

    SDL_SetRenderDrawColor(renderer, 127, 25, 230, 255);
    for (const AABB &toggle_gravity_zone: toggleGravityZones) {
        // Draw only the outline of the toggle gravity zone
        SDL_FRect toggle_gravity_zone_rect = {toggle_gravity_zone.getX() - camera.x, toggle_gravity_zone.getY() - camera.y, toggle_gravity_zone.getWidth(), toggle_gravity_zone.getHeight()};
        SDL_RenderDrawRectF(renderer, &toggle_gravity_zone_rect);
    }

    SDL_SetRenderDrawColor(renderer, 58, 92, 217, 255);
    for (const AABB &increase_fall_speed_zone: increaseFallSpeedZones) {
        // Draw only the outline of the increase fall speed zone
        SDL_FRect increase_fall_speed_zone_rect = {increase_fall_speed_zone.getX() - camera.x, increase_fall_speed_zone.getY() - camera.y, increase_fall_speed_zone.getWidth(), increase_fall_speed_zone.getHeight()};
        SDL_RenderDrawRectF(renderer, &increase_fall_speed_zone_rect);
    }
}

void Level::renderAsteroids(SDL_Renderer *renderer, Point camera) {
    for (Asteroid &asteroid : asteroids) {
        asteroid.render(renderer, camera);
    }
}

void Level::renderAsteroidsDebug(SDL_Renderer *renderer, Point camera) const {
    for (Asteroid const &asteroid : asteroids) {
        asteroid.renderDebug(renderer, camera);
    }
}

void Level::renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const {
    // Draw the 1D moving platforms
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (const MovingPlatform1D &platform: movingPlatforms1D) {
        SDL_FRect platform_rect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }

    // Draw the 2D moving platforms
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    for (const MovingPlatform2D &platform: movingPlatforms2D) {
        SDL_FRect platform_rect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }

    // Draw the switching platforms
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    for (const SwitchingPlatform &platform: switchingPlatforms) {
        SDL_FRect platform_rect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platform_rect);
    }
}

void Level::renderItemsDebug(SDL_Renderer *renderer, Point camera) const {

    // Draw the size power-ups
    SDL_SetRenderDrawColor(renderer, 0, 255, 180, 255);
    for (const SizePowerUp &item : sizePowerUp) {
        item.renderDebug(renderer, camera);
    }

    // Draw the speed power-ups
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);
    for (const SpeedPowerUp &item : speedPowerUp) {
        item.renderDebug(renderer, camera);
    }
}

void Level::applyAsteroidsMovement(double delta_time) {
    // Apply movement to all players
    for (Asteroid &asteroid: asteroids) {
        asteroid.applyMovement(delta_time);
    }
}

void Level::applyPlatformsMovement(double delta_time) {
    // Apply movement for 1D platforms
    for (MovingPlatform1D &platform: movingPlatforms1D) {
        if(platform.getIsMoving()) platform.applyMovement(delta_time);
    }

    // Apply movement for 2D platforms
    for (MovingPlatform2D &platform: movingPlatforms2D) {
        if(platform.getIsMoving()) platform.applyMovement(delta_time);
    }

    // Apply movement for switching platforms
    for (SwitchingPlatform &platform: switchingPlatforms) {
        if(platform.getIsMoving()) platform.applyMovement();
    }
}

void Level::loadMapProperties(const std::string &map_file_name) {
    std::string file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/level.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the properties file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    mapID = j["id"];
    mapName = j["name"];

    // Load spawn points
    for (const auto &spawn_point : j["spawnPoints"]) {
        spawnPoints.emplace_back(std::array<Point, 4>{{
               Point(spawn_point[0][0], spawn_point[0][1]),
               Point(spawn_point[1][0], spawn_point[1][1]),
               Point(spawn_point[2][0], spawn_point[2][1]),
               Point(spawn_point[3][0], spawn_point[3][1])
        }});
    }

    // Load musics
    for (const auto &music : j["musics"]) {
        musics.emplace_back(music);
    }

    std::cout << "Level: Loaded map properties." << std::endl;
}

int Level::loadPolygonsFromJson(const nlohmann::json &json_data, const std::string &zone_name, std::vector<Polygon> &zones, PolygonType type) {
    for (const auto &polygon : json_data[zone_name]) {
        std::vector<Point> vertices;
        for (const auto &vertex : polygon) {
            vertices.emplace_back(vertex[0], vertex[1]);
        }
        zones.emplace_back(vertices, type);
    }

    return (int)json_data[zone_name].size();
}

int Level::loadAABBFromJson(const nlohmann::json &json_data, const std::string &zone_name, std::vector<AABB> &zones, AABBType type) {
    for (const auto &zone : json_data[zone_name]) {
        float x = zone["x"];
        float y = zone["y"];
        float width = zone["width"];
        float height = zone["height"];
        zones.emplace_back(x, y, width, height, type);
    }

    return (int)json_data[zone_name].size();
}

void Level::loadPolygonsFromMap(const std::string &map_file_name) {
    collisionZones.clear();
    iceZones.clear();
    sandZones.clear();
    deathZones.clear();
    cinematicZones.clear();
    bossZones.clear();
    eventZones.clear();
    saveZones.clear();
    toggleGravityZones.clear();
    increaseFallSpeedZones.clear();

    std::string polygons_file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/polygons.json";
    std::string aabbs_file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/aabbs.json";
    std::ifstream polygons_file(polygons_file_path);
    std::ifstream aabbs_file(aabbs_file_path);

    if (!polygons_file.is_open()) {
        std::cerr << "Level: Unable to open the polygons file. Please check the file path." << std::endl;
        return;
    }

    if (!aabbs_file.is_open()) {
        std::cerr << "Level: Unable to open the aabbs file. Please check the file path." << std::endl;
        return;
    }

    using enum PolygonType;
    nlohmann::json polygons;
    polygons_file >> polygons;
    polygons_file.close();

    int collision_zones_size = loadPolygonsFromJson(polygons, "collisionZones", collisionZones, COLLISION);
    int ice_zones_size = loadPolygonsFromJson(polygons, "iceZones", iceZones, ICE);
    int sand_zones_size = loadPolygonsFromJson(polygons, "sandZones", sandZones, SAND);
    int death_zones_size = loadPolygonsFromJson(polygons, "deathZones", deathZones, DEATH);
    int cinematic_zones_size = loadPolygonsFromJson(polygons, "cinematicZones", cinematicZones, CINEMATIC);
    int boss_zones_size = loadPolygonsFromJson(polygons, "bossZones", bossZones, BOSS);
    int event_zones_size = loadPolygonsFromJson(polygons, "eventZones", eventZones, EVENT);
    std::cout << "Level: Loaded " << collision_zones_size << " collision zones, " << ice_zones_size << " ice zones, " << sand_zones_size << " sand zones, " << death_zones_size << " death zones, " << cinematic_zones_size << " cinematic zones, " << boss_zones_size << " boss zones and " << event_zones_size << " event zones." << std::endl;

    using enum AABBType;
    nlohmann::json aabbs;
    aabbs_file >> aabbs;
    aabbs_file.close();

    int save_zones_size = loadAABBFromJson(aabbs, "saveZones", saveZones, SAVE);
    int toggle_gravity_zones_size = loadAABBFromJson(aabbs, "toggleGravityZones", toggleGravityZones, TOGGLE_GRAVITY);
    int increase_fall_speed_zones_size = loadAABBFromJson(aabbs, "increaseFallSpeedZones", increaseFallSpeedZones, INCREASE_FALL_SPEED);
    std::cout << "Level: Loaded " << save_zones_size << " save zones, " << toggle_gravity_zones_size << " toggle gravity zones and " << increase_fall_speed_zones_size << " increase fall speed zones." << std::endl;
}

void Level::loadPlatformsFromMap(const std::string &mapFileName) {
    std::string file_path = std::string(MAPS_DIRECTORY) + mapFileName + "/platforms.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the platforms file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    // Load all 1D moving platforms
    for (const auto &platform : j["movingPlatforms1D"]) {
        float x = platform["x"];
        float y = platform["y"];
        float width = platform["width"];
        float height = platform["height"];
        float speed = platform["speed"];
        float minX = platform["min"];
        float maxX = platform["max"];
        bool start = platform["start"];
        bool axis = platform["axis"];
        movingPlatforms1D.emplace_back(x, y, width, height, speed, minX, maxX, start, axis);
    }

    // Load all 2D moving platforms
    for (const auto &platform : j["movingPlatforms2D"]) {
        float x = platform["x"];
        float y = platform["y"];
        float width = platform["width"];
        float height = platform["height"];
        float speed = platform["speed"];
        Point left(platform["left"][0], platform["left"][1]);
        Point right(platform["right"][0], platform["right"][1]);
        bool start = platform["start"];
        movingPlatforms2D.emplace_back(x, y, width, height, speed, left, right, start);
    }

    // Load all switching platforms
    for (const auto &platform : j["switchingPlatforms"]) {
        float x = platform["x"];
        float y = platform["y"];
        float width = platform["width"];
        float height = platform["height"];
        Uint32 bpm = platform["bpm"];
        std::vector<Point> steps;
        for (const auto &step : platform["steps"]) {
            steps.emplace_back(step[0], step[1]);
        }
        switchingPlatforms.emplace_back(x, y, width, height, bpm, steps);
    }

    std::cout << "Level: Loaded " << movingPlatforms1D.size() << " 1D moving platforms, " << movingPlatforms2D.size() << " 2D moving platforms and " << switchingPlatforms.size() << " switching platforms." << std::endl;
}

void Level::loadItemsFromMap(const std::string &mapFileName) {
    sizePowerUp.clear();
    speedPowerUp.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + mapFileName + "/items.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the items file. Please check the file path." << std::endl;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    // Load all SizePowerUp items
    for (const auto &item : j["sizePowerUp"]) {
        float x = item["x"];
        float y = item["y"];
        float width = item["width"];
        float height = item["height"];
        bool grow = item["grow"];
        sizePowerUp.emplace_back(x, y, width, height, grow);
    }

    // Load all SizePowerUp items
    for (const auto &item : j["speedPowerUp"]) {
        float x = item["x"];
        float y = item["y"];
        float width = item["width"];
        float height = item["height"];
        bool fast = item["fast"];
        speedPowerUp.emplace_back(x, y, width, height, fast);
    }

    std::cout << "Level: Loaded " << sizePowerUp.size() << " size power-up and " << speedPowerUp.size() << " speed power-up." << std::endl;
}