#ifndef PLAY_TOGETHER_BROADPHASEMANAGER_H
#define PLAY_TOGETHER_BROADPHASEMANAGER_H

#include "../Game.h"


class BroadPhaseManager {
private:
    Game *gamePtr; /**< A pointer to the game object. */

    // ZONES
    std::vector<AABB> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<AABB> toggleGravityZones; /**< Collection of polygons representing toggle gravity zones. */
    std::vector<AABB> increaseFallSpeedZones; /**< Collection of polygons representing increase fall speed zones. */

    // PLATFORMS
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */
    std::vector<WeightPlatform> weightPlatforms; /**< Collection of WeightPlatform representing weight platforms. */

    // ITEMS
    std::vector<SizePowerUp> sizePowerUp; /**< Collection of SizePowerUp representing size power-up. */
    std::vector<SpeedPowerUp> speedPowerUp; /**< Collection of SizePowerUp representing size power-up. */


public:

    /* CONSTRUCTORS */

    explicit BroadPhaseManager(Game *game);


    /* ACCESSORS */

    [[nodiscard]] std::vector<AABB> &getSaveZones();
    [[nodiscard]] std::vector<AABB> &getToggleGravityZones();
    [[nodiscard]] std::vector<AABB> &getIncreaseFallSpeedZones();
    [[nodiscard]] std::vector<Polygon> &getDeathZones();
    [[nodiscard]] std::vector<Polygon> &getObstacles();
    [[nodiscard]] std::vector<MovingPlatform1D> &getMovingPlatforms1D();
    [[nodiscard]] std::vector<MovingPlatform2D> &getMovingPlatforms2D();
    [[nodiscard]] std::vector<SwitchingPlatform> &getSwitchingPlatforms();
    [[nodiscard]] std::vector<WeightPlatform> &getWeightPlatforms();
    [[nodiscard]] std::vector<SizePowerUp> &getSizePowerUp();
    [[nodiscard]] std::vector<SpeedPowerUp> &getSpeedPowerUp();


    /* METHODS */

    /**
     * @brief Broad phase collision detection, detects objects that could potentially collide with each other.
     */
    void broadPhase();


private:

    /* SUB METHODS */
    void checkSavesZones(const SDL_FRect& broad_phase_area);
    void checkToggleGravityZones(const SDL_FRect& broad_phase_area);
    void checkIncreaseFallSpeedZones(const SDL_FRect& broad_phase_area);
    void checkDeathZones(const std::vector<Point>& broad_phase_area);
    void checkObstacles(const std::vector<Point>& broad_phase_area);
    void check1DMovingPlatforms(const SDL_FRect& broad_phase_area);
    void check2DMovingPlatforms(const SDL_FRect& broad_phase_area);
    void checkSwitchingPlatforms(const SDL_FRect& broad_phase_area);
    void checkWeightPlatforms(const SDL_FRect& broad_phase_area);
    void checkPowerUp(const SDL_FRect& broad_phase_area);

};


#endif //PLAY_TOGETHER_BROADPHASEMANAGER_H
