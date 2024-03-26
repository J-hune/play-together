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
}


/** ACCESSORS **/

int Level::getMapID() const {
    return mapID;
}

std::string Level::getMapName() const {
    return mapName;
}

std::array<Point, 4> Level::getSpawnPoints() const {
    return spawnPoints;
}

std::vector<Polygon> Level::getCollisionZones() const {
    return collisionZones;
}

std::vector<Polygon> Level::getDeathZones() const {
    return deathZones;
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


/** METHODS **/

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
}

void Level::applyPlatformsMovement() {
    // Apply movement for 1D platforms
    for (MovingPlatform1D &platform: movingPlatforms1D) {
        platform.applyMovement();
    }

    // Apply movement for 2D platforms
    for (MovingPlatform2D &platform: movingPlatforms2D) {
        platform.applyMovement();
    }

    // Apply movement for switching platforms
    for (SwitchingPlatform &platform: switchingPlatforms) {
        platform.applyMovement();
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

void Level::loadMapProperties(const std::string& mapFileName) {
    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/level.json";
    std::ifstream file(filePath);

    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        mapID = j["id"];
        mapName = j["name"];
        spawnPoints = {
            Point(j["spawnPoints"][0][0], j["spawnPoints"][0][1]),
            Point(j["spawnPoints"][1][0], j["spawnPoints"][1][1]),
            Point(j["spawnPoints"][2][0], j["spawnPoints"][2][1]),
            Point(j["spawnPoints"][3][0], j["spawnPoints"][3][1])
        };
        std::cout << "Level: Loaded map properties." << std::endl;
    } else {
        std::cerr << "Level: Unable to open the properties file. Please check the file path." << std::endl;
    }
}

int Level::loadPolygonsFromJson(const nlohmann::json& jsonData, const std::string& zoneName, std::vector<Polygon> &zones, zoneType type) {
    for (const auto& polygon : jsonData[zoneName]) {
        std::vector<Point> vertices;
        for (const auto& vertex : polygon) {
            vertices.emplace_back(vertex[0], vertex[1]);
        }
        zones.emplace_back(vertices, type);
    }

    return (int)jsonData[zoneName].size();
}

void Level::loadPolygonsFromMap(const std::string& mapFileName) {
    collisionZones.clear();
    iceZones.clear();
    sandZones.clear();
    deathZones.clear();
    cinematicZones.clear();
    bossZones.clear();
    eventZones.clear();

    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/polygons.json";
    std::ifstream file(filePath);

    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        int collisionZoneSize = loadPolygonsFromJson(j, "collisionZones", collisionZones, zoneType::COLLISION);
        int iceZonesSize = loadPolygonsFromJson(j, "iceZones", iceZones, zoneType::ICE);
        int sandZonesSize = loadPolygonsFromJson(j, "sandZones", sandZones, zoneType::SAND);
        int deathZonesSize = loadPolygonsFromJson(j, "deathZones", deathZones, zoneType::DEATH);
        int cinematicZonesSize = loadPolygonsFromJson(j, "cinematicZones", cinematicZones, zoneType::CINEMATIC);
        int bossZonesSize = loadPolygonsFromJson(j, "bossZones", bossZones, zoneType::BOSS);
        int eventZonesSize = loadPolygonsFromJson(j, "eventZones", eventZones, zoneType::EVENT);

        std::cout << "Level: Loaded " << collisionZoneSize << " collision zones, " << iceZonesSize << " ice zones, " << sandZonesSize << " sand zones, " << deathZonesSize << " death zones, " << cinematicZonesSize << " cinematic zones, " << bossZonesSize << " boss zones, " << eventZonesSize << " event zones." << std::endl;
    } else {
        std::cerr << "Level: Unable to open the polygons file. Please check the file path." << std::endl;
    }
}

void Level::loadPlatformsFromMap(const std::string& mapFileName) {
    std::string filePath = std::string(MAPS_DIRECTORY) + mapFileName + "/platforms.json";
    std::ifstream file(filePath);

    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        for (const auto& platform : j["movingPlatforms1D"]) {
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

        for (const auto& platform : j["movingPlatforms2D"]) {
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

        for (const auto& platform : j["switchingPlatforms"]) {
            float x = platform["x"];
            float y = platform["y"];
            float width = platform["width"];
            float height = platform["height"];
            Uint32 bpm = platform["bpm"];
            std::vector<Point> steps;
            for (const auto& step : platform["steps"]) {
                steps.emplace_back(step[0], step[1]);
            }
            switchingPlatforms.emplace_back(x, y, width, height, bpm, steps);
        }

        std::cout << "Level: Loaded " << movingPlatforms1D.size() << " 1D moving platforms, " << movingPlatforms2D.size() << " 2D moving platforms and " << switchingPlatforms.size() << " switching platforms." << std::endl;
    } else {
        std::cerr << "Level: Unable to open the platforms file. Please check the file path." << std::endl;
    }
}