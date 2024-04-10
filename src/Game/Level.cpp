#include "../../include/Game/Level.h"

/**
 * @file Level.cpp
 * @brief Implements the Level class responsible for level object.
 */

/** CONSTRUCTOR **/

Level::Level(const std::string &map_name, SDL_Renderer *renderer) {
    std::cout << "Level: Loading level " << map_name << "..." << std::endl;

    // Initialize managers
    textureManager = std::make_unique<TextureManager>(this);

    loadMapProperties(map_name);

    // Load textures
    textureManager->loadMiddlegroundTexture(renderer);
    if (textureManager->getReloadTexture()) textureManager->loadWorldTextures(renderer);

    // Load map environment
    loadEnvironmentFromMap(map_name);
    loadPolygonsFromMap(map_name);
    loadPlatformsFromMap(map_name);
    loadItemsFromMap(map_name);
}


/** ACCESSORS **/

int Level::getWorldID() const {
    return worldID;
}

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

void Level::renderBackgrounds(SDL_Renderer *renderer, const Point camera) const {
    for (const Layer &layer: backgrounds) {
        layer.render(renderer, &camera);
    }
}

void Level::renderMiddleground(SDL_Renderer *renderer, const Point camera) const {
    middleground.render(renderer, &camera);
}

void Level::renderForegrounds(SDL_Renderer *renderer, const Point camera) const {
    for (const Layer &layer: foregrounds) {
        layer.render(renderer, &camera);
    }
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
    for (const Polygon &save_zone: saveZones) {
        for (size_t i = 0; i < save_zone.getVertices().size(); ++i) {
            std::vector<Point> vertices = save_zone.getVertices();
            const auto &vertex1 = vertices[i];
            const auto &vertex2 = vertices[(i + 1) % vertices.size()];
            SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
        }
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

void Level::renderPlatforms(SDL_Renderer *renderer, Point camera) const {
    for (const MovingPlatform1D &platform: movingPlatforms1D) platform.render(renderer, camera);
    for (const MovingPlatform2D &platform: movingPlatforms2D) platform.render(renderer, camera);
    for (const SwitchingPlatform &platform: switchingPlatforms) platform.render(renderer, camera);
}

void Level::renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const {
    for (const MovingPlatform1D &platform: movingPlatforms1D) platform.renderDebug(renderer, camera);
    for (const MovingPlatform2D &platform: movingPlatforms2D) platform.renderDebug(renderer, camera);
    for (const SwitchingPlatform &platform: switchingPlatforms) platform.renderDebug(renderer, camera);

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
        if(platform.getIsMoving()) platform.applyMovement(delta_time);
    }
}


void Level::loadMapProperties(const std::string &map_file_name) {
    musics.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/level.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the properties file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    textureManager->setReloadTexture(worldID != j["worldID"]);
    worldID = j["worldID"];
    mapID = j["levelID"];
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

void Level::loadEnvironmentFromMap(const std::string &map_file_name) {
    backgrounds.clear();
    foregrounds.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/environment.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the environment file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    const std::vector<SDL_Texture*>& background_textures = textureManager->getBackgrounds();
    const std::vector<SDL_Texture*>& foreground_textures = textureManager->getForegrounds();

    // Load background layers
    for (const auto& layer : j["backgroundLayers"]) {
        int texture_id = layer["textureID"];
        int layer_index = layer["layerIndex"];
        backgrounds.emplace_back(*background_textures[texture_id], layer_index);
    }

    // Load middle ground layer
    middleground = Layer(*textureManager->getMiddleground(), 1);

    // Load foreground layers
    for (const auto& layer : j["foregroundLayers"]) {
        int texture_id = layer["textureID"];
        int layer_index = layer["layerIndex"];
        foregrounds.emplace_back(*foreground_textures[texture_id], layer_index);
    }

    std::cout << "Level: Loaded background, middleground and foreground layers." << std::endl;
}

int Level::loadPolygonsFromJson(const nlohmann::json &json_data, const std::string &zone_name, std::vector<Polygon> &zones, ZoneType type) {
    for (const auto &polygon : json_data[zone_name]) {
        std::vector<Point> vertices;
        for (const auto &vertex : polygon) {
            vertices.emplace_back(vertex[0], vertex[1]);
        }
        zones.emplace_back(vertices, type);
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
    saveZones.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + map_file_name + "/polygons.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the polygons file. Please check the file path." << std::endl;
        return;
    }

    using enum ZoneType;
    nlohmann::json j;
    file >> j;
    file.close();

    int collision_zones_size = loadPolygonsFromJson(j, "collisionZones", collisionZones, COLLISION);
    int ice_zones_size = loadPolygonsFromJson(j, "iceZones", iceZones, ICE);
    int sand_zones_size = loadPolygonsFromJson(j, "sandZones", sandZones, SAND);
    int death_zones_size = loadPolygonsFromJson(j, "deathZones", deathZones, DEATH);
    int cinematic_zones_size = loadPolygonsFromJson(j, "cinematicZones", cinematicZones, CINEMATIC);
    int boss_zones_size = loadPolygonsFromJson(j, "bossZones", bossZones, BOSS);
    int event_zones_size = loadPolygonsFromJson(j, "eventZones", eventZones, EVENT);
    int save_zones_size = loadPolygonsFromJson(j, "saveZones", saveZones, SAVE);

    std::cout << "Level: Loaded " << collision_zones_size << " collision zones, " << ice_zones_size << " ice zones, " << sand_zones_size << " sand zones, " << death_zones_size << " death zones, " << cinematic_zones_size << " cinematic zones, " << boss_zones_size << " boss zones, " << event_zones_size << " event zones and " << save_zones_size << " save zones." << std::endl;
}

void Level::loadPlatformsFromMap(const std::string &mapFileName) {
    movingPlatforms1D.clear();
    movingPlatforms2D.clear();
    switchingPlatforms.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + mapFileName + "/platforms.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the platforms file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    const std::vector<Texture> &textures = textureManager->getPlatforms();

    // Load all 1D moving platforms
    for (const auto &platform : j["movingPlatforms1D"]) {
        float x = platform["x"];
        float y = platform["y"];
        float width = platform["width"];
        float height = platform["height"];
        float speed = platform["speed"];
        float min_x = platform["min"];
        float max_x = platform["max"];
        bool start = platform["start"];
        bool axis = platform["axis"];
        int texture_id = platform["texture"];
        movingPlatforms1D.emplace_back(x, y, width, height, textures[texture_id], speed, min_x, max_x, start, axis);
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
        int texture_id = platform["texture"];
        movingPlatforms2D.emplace_back(x, y, width, height, textures[texture_id], speed, left, right, start);
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
        int texture_id = platform["texture"];
        switchingPlatforms.emplace_back(x, y, width, height, textures[texture_id], bpm, steps);
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