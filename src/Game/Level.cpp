#include "../../include/Game/Level.h"

/**
 * @file Level.cpp
 * @brief Implements the Level class responsible for level object.
 */


/* CONSTRUCTOR */

Level::Level(const std::string &map_name, SDL_Renderer *renderer, TextureManager *textureManager) : textureManagerPtr(textureManager) {
    std::cout << "Level: Loading level " << map_name << "..." << std::endl;

    loadMapProperties(map_name);

    // Load textures if needed
    if (textureManager->getWorldID() != worldID) textureManager->loadWorldTextures(renderer, worldID);
    textureManager->loadMiddlegroundTexture(renderer, mapID);

    // Load map environment
    loadEnvironmentFromMap(map_name);
    loadPolygonsFromMap(map_name);
    loadPlatformsFromMap(map_name);
    loadTrapsFromMap(map_name);
    loadItemsFromMap(map_name);
}


/* ACCESSORS */

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

std::vector<MovingPlatform1D>& Level::getMovingPlatforms1D() {
    return movingPlatforms1D;
}

std::vector<MovingPlatform2D>& Level::getMovingPlatforms2D() {
    return movingPlatforms2D;
}

std::vector<SwitchingPlatform> Level::getSwitchingPlatforms() const {
    return switchingPlatforms;
}

std::vector<WeightPlatform>& Level::getWeightPlatforms() {
    return weightPlatforms;
}

std::vector<Treadmill>& Level::getTreadmills() {
    return treadmills;
}

std::vector<Crusher>& Level::getCrushers() {
    return crushers;
}

std::vector<SizePowerUp> Level::getSizePowerUp() const {
    return sizePowerUp;
}

std::vector<SpeedPowerUp> Level::getSpeedPowerUp() const {
    return speedPowerUp;
}

std::vector<Coin> Level::getCoins() const {
    return coins;
}

short Level::getLastCheckpoint() const {
    return lastCheckpoint;
}


/* MUTATORS */

void Level::setLastCheckpoint(short checkpoint) {
    lastCheckpoint = checkpoint;
}

void Level::setAsteroids(std::vector<Asteroid> const &value) {
    this->asteroids = value;
}

void Level::increaseWeightForPlatform(const WeightPlatform &platform) {
    // Search the platform and increase its weight
    auto it = std::ranges::find(weightPlatforms, platform);
    if (it != weightPlatforms.end()) {
        it->increaseWeight();
    }
}

void Level::decreaseWeightForPlatform(const WeightPlatform &platform) {
    // Search the platform and decrease its weight
    auto it = std::ranges::find(weightPlatforms, platform);
    if (it != weightPlatforms.end()) {
        it->decreaseWeight();
    }
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

void Level::removeItemFromCoins(Coin const &item) {
    // Search the item and remove it
    auto it = std::ranges::find(coins, item);
    if (it != coins.end()) {
        coins.erase(it);
    }
}


/* METHODS */

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

void Level::togglePlatformsMovement(bool state){
    for (MovingPlatform1D &platform: movingPlatforms1D) platform.setIsMoving(state);
    for (MovingPlatform2D &platform: movingPlatforms2D) platform.setIsMoving(state);
    for (SwitchingPlatform &platform: switchingPlatforms) platform.setIsMoving(state);
    for (WeightPlatform &platform: weightPlatforms) platform.setIsMoving(state);
    for (Treadmill &treadmill: treadmills) treadmill.setIsMoving(state);
}

void Level::toggleCrushersMovement(bool state){
    for (Crusher &crusher: crushers) crusher.setIsMoving(state);
}

void Level::applyAsteroidsMovement(double delta_time) {
    // Apply movement to all players
    for (Asteroid &asteroid: asteroids) {
        asteroid.applyMovement(delta_time);
    }
}

void Level::applyPlatformsMovement(double delta_time) {
    for (MovingPlatform1D &platform: movingPlatforms1D) platform.applyMovement(delta_time); // Apply movement for 1D platforms
    for (MovingPlatform2D &platform: movingPlatforms2D) platform.applyMovement(delta_time); // Apply movement for 2D platforms
    for (SwitchingPlatform &platform: switchingPlatforms) platform.applyMovement(delta_time); // Apply movement for switching platforms
    for (WeightPlatform &platform: weightPlatforms) platform.applyMovement(delta_time); // Apply movement for weight platforms
}

bool Level::applyTrapsMovement(double delta_time) {
    bool check = false;

    // Apply movement to all crushers
    for (Crusher &crusher: crushers) {
        if (crusher.applyMovement(delta_time)) check = true;
    }

    return check;
}

void Level::renderBackgrounds(SDL_Renderer *renderer, const Point camera) const {
    for (const Layer &layer: backgrounds) {
        layer.render(renderer, &camera);
    }
}

void Level::renderMiddleground(SDL_Renderer *renderer, const Point camera) const {
    SDL_Rect src_rect = middleground.getSize();
    SDL_FRect layer_rect_1 = { -40 - camera.x, -350 - camera.y, static_cast<float>(src_rect.w), static_cast<float>(src_rect.h)}; // TODO: change static values to 0
    SDL_RenderCopyExF(renderer, middleground.getTexture(), &src_rect, &layer_rect_1, 0.0, nullptr, SDL_FLIP_NONE);
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

void Level::renderPlatforms(SDL_Renderer *renderer, Point camera) {
    for (const MovingPlatform1D &platform: movingPlatforms1D) platform.render(renderer, camera);
    for (const MovingPlatform2D &platform: movingPlatforms2D) platform.render(renderer, camera);
    for (const SwitchingPlatform &platform: switchingPlatforms) platform.render(renderer, camera);
    for (const WeightPlatform &platform: weightPlatforms) platform.render(renderer, camera);
    for (Treadmill &treadmill: treadmills) treadmill.render(renderer, camera);
}

void Level::renderPlatformsDebug(SDL_Renderer *renderer, Point camera) const {
    for (const MovingPlatform1D &platform: movingPlatforms1D) platform.renderDebug(renderer, camera);
    for (const MovingPlatform2D &platform: movingPlatforms2D) platform.renderDebug(renderer, camera);
    for (const SwitchingPlatform &platform: switchingPlatforms) platform.renderDebug(renderer, camera);
    for (const WeightPlatform &platform: weightPlatforms) platform.renderDebug(renderer, camera);
    for (const Treadmill &treadmill: treadmills) treadmill.renderDebug(renderer, camera);

}

void Level::renderTraps(SDL_Renderer *renderer, Point camera) const {
    for (const Crusher &crusher: crushers) crusher.render(renderer, camera); // Draw the crushers
}

void Level::renderTrapsDebug(SDL_Renderer *renderer, Point camera) const {
    for (const Crusher &crusher: crushers) crusher.renderDebug(renderer, camera); // Draw the crushers
}

void Level::renderItems(SDL_Renderer *renderer, Point camera) {
    for (SizePowerUp &item : sizePowerUp) item.render(renderer, camera); // Draw the size power-ups
    for (SpeedPowerUp &item : speedPowerUp) item.render(renderer, camera); // Draw the speed power-ups
    for (Coin &item : coins) item.render(renderer, camera); // Draw the coins
}

void Level::renderItemsDebug(SDL_Renderer *renderer, Point camera) const {

    // Draw the size power-ups
    SDL_SetRenderDrawColor(renderer, 0, 255, 180, 255);
    for (const SizePowerUp &item : sizePowerUp) item.renderDebug(renderer, camera);

    // Draw the speed power-ups
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    for (const SpeedPowerUp &item : speedPowerUp) item.renderDebug(renderer, camera);

    // Draw the coins
    SDL_SetRenderDrawColor(renderer, 255, 255, 64, 255);
    for (const Coin &item : coins) item.renderDebug(renderer, camera);
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

    worldID = j["worldID"];
    mapID = j["levelID"];
    mapName = j["name"];

    // Load spawn points
    for (const auto &spawn_point : j["spawnPoints"]) {
        spawnPoints.push_back({
            Point(spawn_point[0][0], spawn_point[0][1]),
            Point(spawn_point[1][0], spawn_point[1][1]),
            Point(spawn_point[2][0], spawn_point[2][1]),
            Point(spawn_point[3][0], spawn_point[3][1])
        });
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

    const std::vector<SDL_Texture*>& background_textures = textureManagerPtr->getBackgrounds();
    const std::vector<SDL_Texture*>& foreground_textures = textureManagerPtr->getForegrounds();

    // Load background layers
    for (const auto& layer : j["backgroundLayers"]) {
        int texture_id = layer["textureID"];
        int layer_index = layer["layerIndex"];
        backgrounds.emplace_back(*background_textures[texture_id], layer_index);
    }

    // Load middle ground layer
    middleground = Texture(*textureManagerPtr->getMiddleground());

    // Load foreground layers
    for (const auto& layer : j["foregroundLayers"]) {
        int texture_id = layer["textureID"];
        int layer_index = layer["layerIndex"];
        foregrounds.emplace_back(*foreground_textures[texture_id], layer_index);
    }

    std::cout << "Level: Loaded background, middleground and foreground layers." << std::endl;
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
    movingPlatforms1D.clear();
    movingPlatforms2D.clear();
    switchingPlatforms.clear();
    weightPlatforms.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + mapFileName + "/platforms.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the platforms file. Please check the file path." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    const std::vector<Texture> &textures = textureManagerPtr->getPlatforms();

    // Load all 1D moving platforms
    for (const auto &platform : j["movingPlatforms1D"]) {
        float x = platform["x"];
        float y = platform["y"];
        float size = platform["size"];
        float speed = platform["speed"];
        float min_x = platform["min"];
        float max_x = platform["max"];
        bool start = platform["start"];
        bool axis = platform["axis"];
        int texture_id = platform["texture"];
        movingPlatforms1D.emplace_back(x, y, size, speed, min_x, max_x, start, axis, textures[texture_id]);
    }

    // Load all 2D moving platforms
    for (const auto &platform : j["movingPlatforms2D"]) {
        float x = platform["x"];
        float y = platform["y"];
        float size = platform["size"];
        float speed = platform["speed"];
        Point left(platform["left"][0], platform["left"][1]);
        Point right(platform["right"][0], platform["right"][1]);
        bool start = platform["start"];
        int texture_id = platform["texture"];
        movingPlatforms2D.emplace_back(x, y, size, speed, left, right, start, textures[texture_id]);
    }

    // Load all switching platforms
    for (const auto &platform : j["switchingPlatforms"]) {
        float x = platform["x"];
        float y = platform["y"];
        float size = platform["size"];
        Uint32 bpm = platform["bpm"];
        std::vector<Point> steps;
        for (const auto &step : platform["steps"]) {
            steps.emplace_back(step[0], step[1]);
        }
        int texture_id = platform["texture"];
        switchingPlatforms.emplace_back(x, y, size, bpm, steps, textures[texture_id]);
    }

    // Load all weight platforms
    for (const auto &platform : j["weightPlatforms"]) {
        float x = platform["x"];
        float y = platform["y"];
        float size = platform["size"];
        float stepDistance = platform["stepDistance"];
        int texture_id = platform["texture"];
        weightPlatforms.emplace_back(x, y, size, stepDistance, textures[texture_id]);
    }

    // Load all treadmills
    for (const auto &treadmill : j["treadmills"]) {
        float x = treadmill["x"];
        float y = treadmill["y"];
        float size = treadmill["size"];
        float speed = treadmill["speed"];
        float direction = treadmill["direction"];
        Uint32 spriteSpeed = treadmill["spriteSpeed"];
        treadmills.emplace_back(x, y, size, speed, direction, spriteSpeed);
    }

    std::cout << "Level: Loaded " << movingPlatforms1D.size() << " 1D moving platforms, " << movingPlatforms2D.size() << " 2D moving platforms, " << switchingPlatforms.size() << " switching platforms and " << weightPlatforms.size() << "weight platforms." << std::endl;
}

void Level::loadTrapsFromMap(const std::string &mapFileName) {
    crushers.clear();

    std::string file_path = std::string(MAPS_DIRECTORY) + mapFileName + "/traps.json";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Level: Unable to open the traps file. Please check the file path." << std::endl;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    const std::vector<Texture> &textures = textureManagerPtr->getCrushers();

    // Load all crushers
    for (const auto &crusher : j["crushers"]) {
        float x = crusher["x"];
        float y = crusher["y"];
        float size = crusher["size"];
        float min = crusher["min"];
        float max = crusher["max"];
        Uint32 moveUpTime = crusher["moveUpTime"];
        Uint32 waitUpTime = crusher["waitUpTime"];
        Uint32 waitDownTime = crusher["waitDownTime"];
        int texture_id = crusher["texture"];
        crushers.emplace_back(x, y, size, min, max, moveUpTime, waitUpTime, waitDownTime, textures[texture_id]);
    }

    std::cout << "Level: Loaded " << crushers.size() << " crushers." << std::endl;
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

    // Load all Coin items
    for (const auto &item : j["coins"]) {
        float x = item["x"];
        float y = item["y"];
        float width = item["width"];
        float height = item["height"];
        int value  = item["value"];
        coins.emplace_back(x, y, width, height, value);
    }

    std::cout << "Level: Loaded " << sizePowerUp.size() << " size power-up, " << speedPowerUp.size() << " speed power-up and " << coins.size() << " coins." << std::endl;
}