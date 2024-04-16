#include "../../../include/Game/GameManagers/BroadPhaseManager.h"


/**
 * @file BroadPhaseManager.cpp
 * @brief Implements the BroadPhaseManager class responsible for the collision broad phase.
 */

/* CONSTRUCTORS */

BroadPhaseManager::BroadPhaseManager(Game *game) : gamePtr(game) {}


/* ACCESSORS */

std::vector<AABB> &BroadPhaseManager::getSaveZones() {
    return saveZones;
}

std::vector<AABB> &BroadPhaseManager::getToggleGravityZones() {
    return toggleGravityZones;
}

std::vector<AABB> &BroadPhaseManager::getIncreaseFallSpeedZones() {
    return increaseFallSpeedZones;
}

std::vector<Polygon> &BroadPhaseManager::getDeathZones() {
    return deathZones;
}

std::vector<Polygon> &BroadPhaseManager::getObstacles() {
    return obstacles;
}

std::vector<MovingPlatform1D> &BroadPhaseManager::getMovingPlatforms1D() {
    return movingPlatforms1D;
}

std::vector<MovingPlatform2D> &BroadPhaseManager::getMovingPlatforms2D() {
    return movingPlatforms2D;
}

std::vector<SwitchingPlatform> &BroadPhaseManager::getSwitchingPlatforms() {
    return switchingPlatforms;
}

std::vector<WeightPlatform> &BroadPhaseManager::getWeightPlatforms() {
    return weightPlatforms;
}

std::vector<Crusher> &BroadPhaseManager::getCrushers() {
    return crushers;
}

std::vector<SizePowerUp> &BroadPhaseManager::getSizePowerUps() {
    return sizePowerUp;
}

std::vector<SpeedPowerUp> &BroadPhaseManager::getSpeedPowerUps() {
    return speedPowerUp;
}

std::vector<Coin> &BroadPhaseManager::getCoins() {
    return coins;
}


/* METHODS */

void BroadPhaseManager::checkSavesZones(const SDL_FRect& broad_phase_area) {
    saveZones.clear(); // Empty old save zones

    // Check collisions with each save zone
    for (const AABB &aabb: gamePtr->getLevel()->getZones(AABBType::SAVE)) {
        if (checkAABBCollision(broad_phase_area, aabb.getRect())) {
            saveZones.push_back(aabb);
        }
    }
}

void BroadPhaseManager::checkToggleGravityZones(const SDL_FRect &broad_phase_area) {
    toggleGravityZones.clear(); // Empty old toggle gravity zones

    // Check collisions with each toggle gravity zone
    for (const AABB &aabb: gamePtr->getLevel()->getZones(AABBType::TOGGLE_GRAVITY)) {
        if (checkAABBCollision(broad_phase_area, aabb.getRect())) {
            toggleGravityZones.push_back(aabb);
        }
    }
}

void BroadPhaseManager::checkIncreaseFallSpeedZones(const SDL_FRect &broad_phase_area) {
    increaseFallSpeedZones.clear(); // Empty old increase fall speed zones

    // Check collisions with each increase fall speed zone
    for (const AABB &aabb: gamePtr->getLevel()->getZones(AABBType::INCREASE_FALL_SPEED)) {
        if (checkAABBCollision(broad_phase_area, aabb.getRect())) {
            increaseFallSpeedZones.push_back(aabb);
        }
    }
}

void BroadPhaseManager::checkDeathZones(const std::vector<Point>& broad_phase_area) {
    deathZones.clear(); // Empty old death zones

    // Check collisions with each save zone
    for (const Polygon &zone: gamePtr->getLevel()->getZones(PolygonType::DEATH)) {
        if (checkSATCollision(broad_phase_area, zone)) {
            deathZones.push_back(zone);
        }
    }
}

void BroadPhaseManager::checkObstacles(const std::vector<Point> &broad_phase_area) {
    obstacles.clear(); // Empty old death zones

    // Check collisions with each obstacle
    for (const Polygon &obstacle: gamePtr->getLevel()->getZones(PolygonType::COLLISION)) {
        if (checkSATCollision(broad_phase_area, obstacle)) {
            obstacles.push_back(obstacle);
        }
    }
}

void BroadPhaseManager::check1DMovingPlatforms(const SDL_FRect &broad_phase_area) {
    movingPlatforms1D.clear(); // Empty old 1D moving platforms

    // Check for collisions with each 1D moving platform
    for (const MovingPlatform1D &platform: gamePtr->getLevel()->getMovingPlatforms1D()) {
        if (checkAABBCollision(broad_phase_area, platform.getBoundingBox())) {
            movingPlatforms1D.push_back(platform);
        }
    }
}

void BroadPhaseManager::check2DMovingPlatforms(const SDL_FRect &broad_phase_area) {
    movingPlatforms2D.clear(); // Empty old 2D moving platforms

    // Check for collisions with each 2D moving platform
    for (const MovingPlatform2D &platform: gamePtr->getLevel()->getMovingPlatforms2D()) {
        if (checkAABBCollision(broad_phase_area, platform.getBoundingBox())) {
            movingPlatforms2D.push_back(platform);
        }
    }
}

void BroadPhaseManager::checkSwitchingPlatforms(const SDL_FRect &broad_phase_area) {
    switchingPlatforms.clear(); // Empty old switching platforms

    // Check for collisions with each switching platform
    for (const SwitchingPlatform &platform: gamePtr->getLevel()->getSwitchingPlatforms()) {
        if (checkAABBCollision(broad_phase_area, platform.getBoundingBox())) {
            switchingPlatforms.push_back(platform);
        }
    }
}

void BroadPhaseManager::checkWeightPlatforms(const SDL_FRect &broad_phase_area) {
    weightPlatforms.clear(); // Empty old weight platforms

    // Check for collisions with each weight platform
    for (const WeightPlatform &platform: gamePtr->getLevel()->getWeightPlatforms()) {
        if (checkAABBCollision(broad_phase_area, platform.getBoundingBox())) {
            weightPlatforms.push_back(platform);
        }
    }
}

void BroadPhaseManager::checkCrushers(const SDL_FRect &broad_phase_area) {
    crushers.clear(); // Empty old crushers

    // Check for collisions with each crusher
    for (const Crusher &crusher: gamePtr->getLevel()->getCrushers()) {
        if (checkAABBCollision(broad_phase_area, crusher.getBoundingBox())) {
            crushers.push_back(crusher);
        }
    }
}

void BroadPhaseManager::checkPowerUps(const SDL_FRect &broad_phase_area) {
    sizePowerUp.clear(); // Empty old size power-up
    speedPowerUp.clear(); // Empty old speed power-up

    // Check for collisions with size power-up
    for (const SizePowerUp &item: gamePtr->getLevel()->getSizePowerUp()) {
        if (checkAABBCollision(broad_phase_area, item.getBoundingBox())) {
            sizePowerUp.push_back(item);
        }
    }

    // Check for collisions with speed power-up
    for (const SpeedPowerUp &item: gamePtr->getLevel()->getSpeedPowerUp()) {
        if (checkAABBCollision(broad_phase_area, item.getBoundingBox())) {
            speedPowerUp.push_back(item);
        }
    }
}

void BroadPhaseManager::checkCoins(const SDL_FRect &broad_phase_area) {
    coins.clear(); // Empty old coins

    // Check for collisions with coins
    for (const Coin &coin: gamePtr->getLevel()->getCoins()) {
        if (checkAABBCollision(broad_phase_area, coin.getBoundingBox())) {
            coins.push_back(coin);
        }
    }
}

void BroadPhaseManager::broadPhase() {

    std::vector<Point> broad_phase_area_vertices = gamePtr->getCamera()->getBroadPhaseAreaVertices();
    SDL_FRect broad_phase_area_bounding_box = gamePtr->getCamera()->getBroadPhaseArea();

    checkSavesZones(broad_phase_area_bounding_box);
    checkToggleGravityZones(broad_phase_area_bounding_box);
    checkIncreaseFallSpeedZones(broad_phase_area_bounding_box);
    checkDeathZones(broad_phase_area_vertices);
    checkObstacles(broad_phase_area_vertices);
    check1DMovingPlatforms(broad_phase_area_bounding_box);
    check2DMovingPlatforms(broad_phase_area_bounding_box);
    checkSwitchingPlatforms(broad_phase_area_bounding_box);
    checkWeightPlatforms(broad_phase_area_bounding_box);
    checkCrushers(broad_phase_area_bounding_box);
    checkPowerUps(broad_phase_area_bounding_box);
    checkCoins(broad_phase_area_bounding_box);

}