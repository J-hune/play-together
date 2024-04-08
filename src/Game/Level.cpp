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

std::vector<Polygon> Level::getZones(ZoneType type) const {
    switch(type) {
        using enum ZoneType;
        case COLLISION: return collisionZones;
        case ICE: return iceZones;
        case SAND: return sandZones;
        case DEATH: return deathZones;
        case CINEMATIC: return cinematicZones;
        case BOSS: return bossZones;
        case EVENT: return eventZones;
        case SAVE: return saveZones;
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
    size_t i = 0;
    while (i < sizePowerUp.size() && item != sizePowerUp[i]) {
        i++;
    }
    sizePowerUp.erase(sizePowerUp.begin() + i);
}

void Level::removeItemFromSpeedPowerUp(SpeedPowerUp const &item) {
    // Search the item and remove it
    size_t i = 0;
    while (i < speedPowerUp.size() && item != speedPowerUp[i]) {
        i++;
    }
    speedPowerUp.erase(speedPowerUp.begin() + i);
}


/** METHODS **/

void Level::generateAsteroid(int nbAsteroid, Point camera, size_t seed) {
    // Loop to generate asteroids until the desired number is reached
    for (auto i = static_cast<int>(asteroids.size()); i < nbAsteroid; i++){
        // Add a new asteroid to the asteroids vector with coordinates based on the camera position
        Asteroid newAsteroid(camera.x, camera.y, seed);
        asteroids.emplace_back(newAsteroid);

        // Send the asteroid throw the network
        if (Mediator::isServerRunning()) {
            Mediator::sendAsteroidCreation(newAsteroid);
        }
    }
}

void Level::addAsteroid(Asteroid const &asteroid) {
    std::cout << "Level: Adding asteroid to the game. " << asteroids.size() << std::endl;
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
    for (const Polygon &saveZone: saveZones) {
        for (size_t i = 0; i < saveZone.getVertices().size(); ++i) {
            std::vector<Point> vertices = saveZone.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
    }
}

void Level::renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const {
    // Draw the 1D moving platforms
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (const MovingPlatform1D &platform: movingPlatforms1D) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
    }

    // Draw the 2D moving platforms
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    for (const MovingPlatform2D &platform: movingPlatforms2D) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
    }

    // Draw the switching platforms
    SDL_SetRenderDrawColor(renderer, 145, 0, 145, 255);
    for (const SwitchingPlatform &platform: switchingPlatforms) {
        SDL_FRect platformRect = {platform.getX() - camera.x, platform.getY() - camera.y, platform.getW(), platform.getH()};
        SDL_RenderFillRectF(renderer, &platformRect);
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

void Level::applyAsteroidsMovement(double deltaTime) {
    // Apply movement to all players
    for (Asteroid &asteroid: asteroids) {
        asteroid.applyMovement(deltaTime);
    }
}

void Level::applyPlatformsMovement(double deltaTime) {
    // Apply movement for 1D platforms
    for (MovingPlatform1D &platform: movingPlatforms1D) {
        if(platform.getIsMoving()) platform.applyMovement(deltaTime);
    }

    // Apply movement for 2D platforms
    for (MovingPlatform2D &platform: movingPlatforms2D) {
        if(platform.getIsMoving()) platform.applyMovement(deltaTime);
    }

    // Apply movement for switching platforms
    for (SwitchingPlatform &platform: switchingPlatforms) {
        if(platform.getIsMoving()) platform.applyMovement();
    }
}

void Level::loadMapProperties(const std::string &mapFileName) {
    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/level.json";
    std::ifstream file(filePath);

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
    for (const auto &spawnPoint : j["spawnPoints"]) {
        spawnPoints.emplace_back(std::array<Point, 4>{{
               Point(spawnPoint[0][0], spawnPoint[0][1]),
               Point(spawnPoint[1][0], spawnPoint[1][1]),
               Point(spawnPoint[2][0], spawnPoint[2][1]),
               Point(spawnPoint[3][0], spawnPoint[3][1])
        }});
    }

    // Load musics
    for (const auto &music : j["musics"]) {
        musics.emplace_back(music);
    }

    std::cout << "Level: Loaded map properties." << std::endl;
}

int Level::loadPolygonsFromJson(const nlohmann::json &jsonData, const std::string &zoneName, std::vector<Polygon> &zones, ZoneType type) {
    for (const auto &polygon : jsonData[zoneName]) {
        std::vector<Point> vertices;
        for (const auto &vertex : polygon) {
            vertices.emplace_back(vertex[0], vertex[1]);
        }
        zones.emplace_back(vertices, type);
    }

    return (int)jsonData[zoneName].size();
}

void Level::loadPolygonsFromMap(const std::string &mapFileName) {
    collisionZones.clear();
    iceZones.clear();
    sandZones.clear();
    deathZones.clear();
    cinematicZones.clear();
    bossZones.clear();
    eventZones.clear();
    saveZones.clear();
    saveZones.clear();

    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/polygons.json";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the polygons file. Please check the file path." << std::endl;
        return;
    }

    using enum ZoneType;
    nlohmann::json j;
    file >> j;
    file.close();

    int collisionZoneSize = loadPolygonsFromJson(j, "collisionZones", collisionZones, COLLISION);
    int iceZonesSize = loadPolygonsFromJson(j, "iceZones", iceZones, ICE);
    int sandZonesSize = loadPolygonsFromJson(j, "sandZones", sandZones, SAND);
    int deathZonesSize = loadPolygonsFromJson(j, "deathZones", deathZones, DEATH);
    int cinematicZonesSize = loadPolygonsFromJson(j, "cinematicZones", cinematicZones, CINEMATIC);
    int bossZonesSize = loadPolygonsFromJson(j, "bossZones", bossZones, BOSS);
    int eventZonesSize = loadPolygonsFromJson(j, "eventZones", eventZones, EVENT);
    int saveZonesSize = loadPolygonsFromJson(j, "saveZones", saveZones, SAVE);

    std::cout << "Level: Loaded " << collisionZoneSize << " collision zones, " << iceZonesSize << " ice zones, " << sandZonesSize << " sand zones, " << deathZonesSize << " death zones, " << cinematicZonesSize << " cinematic zones, " << bossZonesSize << " boss zones, " << eventZonesSize << " event zones and " << saveZonesSize << " save zones." << std::endl;
}

void Level::loadPlatformsFromMap(const std::string &mapFileName) {
    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/platforms.json";
    std::ifstream file(filePath);

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

    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/items.json";
    std::ifstream file(filePath);

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