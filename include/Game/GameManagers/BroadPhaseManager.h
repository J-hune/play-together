#ifndef PLAY_TOGETHER_BROADPHASEMANAGER_H
#define PLAY_TOGETHER_BROADPHASEMANAGER_H

#include "../Game.h"

/**
 * @file BroadPhaseManager.h
 * @brief Defines the BroadPhaseManager class responsible for the collision broad phase.
 */


class BroadPhaseManager {
private:
    /* ATTRIBUTES */

    Game *gamePtr; /**< A pointer to the game object. */

    // ZONES
    std::vector<AABB> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<AABB> rescueZones; /**< Collection of polygons representing rescue zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<AABB> toggleGravityZones; /**< Collection of polygons representing toggle gravity zones. */
    std::vector<AABB> increaseFallSpeedZones; /**< Collection of polygons representing increase fall speed zones. */

    // LEVERS
    std::vector<TreadmillLever> treadmillLevers; /**< Collection of TreadmillLever representing treadmill levers. */
    std::vector<PlatformLever> platformLevers; /**< Collection of PlatformLever representing platform levers. */
    std::vector<CrusherLever> crusherLevers; /**< Collection of CrusherLever representing crusher levers. */

    // PLATFORMS
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */
    std::vector<WeightPlatform> weightPlatforms; /**< Collection of WeightPlatform representing weight platforms. */
    std::vector<Treadmill> treadmills; /**< Collection of Treadmill representing treadmills. */

    // TRAPS
    std::vector<Crusher> crushers; /**< Collection of Crusher representing crushers. */

    // ITEMS
    std::vector<SizePowerUp> sizePowerUp; /**< Collection of SizePowerUp representing size power-up. */
    std::vector<SpeedPowerUp> speedPowerUp; /**< Collection of SizePowerUp representing size power-up. */
    std::vector<Coin> coins; /**< Collection of Coin representing coins. */
    std::vector<Item*> items; /**< Collection of items. */


public:

    /* CONSTRUCTORS */

    explicit BroadPhaseManager(Game *game);


    /* ACCESSORS */

    [[nodiscard]] std::vector<AABB> &getSaveZones();
    [[nodiscard]] std::vector<AABB> &getRescueZones();
    [[nodiscard]] std::vector<AABB> &getToggleGravityZones();
    [[nodiscard]] std::vector<AABB> &getIncreaseFallSpeedZones();
    [[nodiscard]] std::vector<Polygon> &getDeathZones();
    [[nodiscard]] std::vector<Polygon> &getObstacles();
    [[nodiscard]] std::vector<TreadmillLever> &getTreadmillLevers();
    [[nodiscard]] std::vector<PlatformLever> &getPlatformLevers();
    [[nodiscard]] std::vector<CrusherLever> &getCrusherLevers();
    [[nodiscard]] std::vector<MovingPlatform1D> &getMovingPlatforms1D();
    [[nodiscard]] std::vector<MovingPlatform2D> &getMovingPlatforms2D();
    [[nodiscard]] std::vector<SwitchingPlatform> &getSwitchingPlatforms();
    [[nodiscard]] std::vector<WeightPlatform> &getWeightPlatforms();
    [[nodiscard]] std::vector<Treadmill> &getTreadmills();
    [[nodiscard]] std::vector<Crusher> &getCrushers();
    [[nodiscard]] std::vector<SizePowerUp> &getSizePowerUps();
    [[nodiscard]] std::vector<SpeedPowerUp> &getSpeedPowerUps();
    [[nodiscard]] std::vector<Coin> &getCoins();
    [[nodiscard]] std::vector<Item*> &getItems();


    /* METHODS */

    /**
     * @brief Broad phase collision detection, detects objects that could potentially collide with each other.
     */
    void broadPhase();


private:

    /* SUB METHODS */
    void checkSavesZones(const SDL_FRect& broad_phase_area);
    void checkRescueZones(const SDL_FRect& broad_phase_area);
    void checkToggleGravityZones(const SDL_FRect& broad_phase_area);
    void checkIncreaseFallSpeedZones(const SDL_FRect& broad_phase_area);
    void checkDeathZones(const std::vector<Point>& broad_phase_area);
    void checkObstacles(const std::vector<Point>& broad_phase_area);
    void checkTreadmillLevers(const SDL_FRect& broad_phase_area);
    void checkPlatformLevers(const SDL_FRect& broad_phase_area);
    void checkCrusherLevers(const SDL_FRect& broad_phase_area);
    void check1DMovingPlatforms(const SDL_FRect& broad_phase_area);
    void check2DMovingPlatforms(const SDL_FRect& broad_phase_area);
    void checkSwitchingPlatforms(const SDL_FRect& broad_phase_area);
    void checkWeightPlatforms(const SDL_FRect& broad_phase_area);
    void checkTreadmills(const SDL_FRect& broad_phase_area);
    void checkCrushers(const SDL_FRect& broad_phase_area);
    void checkPowerUps(const SDL_FRect& broad_phase_area);
    void checkCoins(const SDL_FRect& broad_phase_area);
    void checkItems(const SDL_FRect& broad_phase_area);

};


#endif //PLAY_TOGETHER_BROADPHASEMANAGER_H
