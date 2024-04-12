#ifndef PLAY_TOGETHER_GAME_H
#define PLAY_TOGETHER_GAME_H

#include <SDL.h>
#include <vector>
#include <climits>
#include <string>
#include <ranges>
#include <cmath>
#include <algorithm>
#include <SDL_ttf.h>
#include "../Physics/CollisionHandler.h"
#include "../Utils/Mediator.h"
#include "GameManagers/InputManager.h"
#include "GameManagers/RenderManager.h"
#include "GameManagers/SaveManager.h"
#include "GameManagers/PlayerManager.h"


/**
 * @file Game.h
 * @brief Defines the Game class responsible for handling the main game logic.
 */

// Forward declaration of InputManager, CollisionHandler
class InputManager;
class RenderManager;
class SaveManager;
class PlayerManager;

/**
 * @class Game
 * @brief Represents the main game logic including initialization, event handling, collision detection, and rendering.
 */
class Game {
public:
    /** CONSTRUCTORS **/

    Game(SDL_Window *window, SDL_Renderer *renderer, int refreshRate, bool *quitFlag);


    /** ACCESSORS **/

    /**
     * @brief Returns the current game state.
     * @return The current game state.
     */
    [[nodiscard]] GameState getGameState() const;

    /**
     * @brief Returns the input manager of the game.
     * @return A pointer of InputManager object representing the input manager of the game.
     */
    [[nodiscard]] InputManager &getInputManager();

    /**
     * @brief Returns the render object of the game.
     * @return A pointer of Renderer object representing the render object of the game.
     */
    [[nodiscard]] RenderManager &getRenderManager();

    /**
     * @brief Returns the save manager of the game.
     * @return A pointer of SaveManager object representing the save manager of the game.
     */
    [[nodiscard]] SaveManager &getSaveManager();

    /**
     * @brief Returns the player manager of the game.
     * @return A pointer of PlayerManager object representing the player manager of the game.
     */
    [[nodiscard]] PlayerManager &getPlayerManager();

    /**
     * @brief Returns the camera of the game.
     * @return A pointer of Camera object representing the camera of the game.
     */
    [[nodiscard]] Camera* getCamera();

    /**
     * @brief Get the level object.
     * @return The level object.
     */
    [[nodiscard]] Level *getLevel();

    /**
     *
     * @brief Get the tick rate of the game.
     * @return The tick rate of the game.
     */
    [[nodiscard]] int getTickRate() const;

    /**
     * @brief Get the frame rate of the game.
     * @return The frame rate of the game.
     */
    [[nodiscard]] int getEffectiveFrameRate() const;


    /** MODIFIERS **/

    /**
     * @brief Set the level attribute.
     * @param map_name The name of the new map.
     */
    void setLevel(std::string const &map_name);

    /**
     * @brief Set a new state to enable_platforms_movement
     * @param state the state of enable_platforms_movement
     */
    void setEnablePlatformsMovement(bool state);

    /**
     * @brief Set the frame rate of the game.
     * @param frameRate The frame rate to set.
     */
    void setFrameRate(int frameRate);

    /**
    * @brief Switch mavity between normal and reversed.
    */
    void switchMavity();


    /** PUBLIC METHODS **/

    /**
     * @brief Initializes the game by loading the level and the character.
     * @param slot The save slot to use when loading the game. (0 by default)
     */
    void initializeHostedGame(int slot = 0);

    /**
     * @brief Updates the game logic in a fixed time step.
     */
    void fixedUpdate();

    /**
     * @brief Updates the game logic.
     * @param deltaTime The time elapsed since the last frame in seconds.
     * @param ratio The ratio of the movement to apply.
     */
    void update(double deltaTime, double ratio);

    /**
     * @brief Runs the game loop.
     */
    void run();

    /**
    * @brief Toggles the pause state of the game.
    */
    void togglePause();

    /**
     * @brief Stops the game loop and exits the game.
     */
    void stop();

    /**
     * @brief Exits the game.
     */
    void exitGame();

private:
    /** ATTRIBUTES **/

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */

    std::unique_ptr<InputManager> inputManager; /**< Input manager for handling input events. */
    std::unique_ptr<RenderManager> renderManager; /**< Renderer object for rendering the game. */
    std::unique_ptr<SaveManager> saveManager; /**< Save manager for saving and loading the game state. */
    std::unique_ptr<PlayerManager> playerManager; /**< Player manager for handling the players in the game. */

    int frameRate = 60; /**< The refresh rate of the game. */
    const int tickRate = 30; /**< The tick rate of the game. */
    int effectiveFrameFps = frameRate; /**< The effective fps. */

    GameState gameState = GameState::STOPPED; /**< The current game state. */
    bool *quitFlagPtr = nullptr; /**< Reference to the quit flag. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    Music music; /**< Represents the music that is currently played in the game. */
    size_t seed;

    int worldID = 1; /**< Represents the ID of the current world. */

    // Broad phase attributes
    std::vector<Polygon> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */
    std::vector<WeightPlatform> weightPlatforms; /**< Collection of WeightPlatform representing weight platforms. */
    std::vector<SizePowerUp> sizePowerUp; /**< Collection of SizePowerUp representing size power-up. */
    std::vector<SpeedPowerUp> speedPowerUp; /**< Collection of SizePowerUp representing size power-up. */

    // Debug variables used for the application console
    bool enable_platforms_movement = true;


    /** PRIVATE METHODS **/

    /**
     * @brief Applies the movement to all players in the game.
     * @param ratio The ratio of the movement to apply.
     */
    void applyPlayersMovement(double ratio);

    /**
     * @brief Calculates the movement of all players in the game.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void calculatePlayersMovement(double deltaTime);

    /**
     * @brief Handles collisions between the asteroid and obstacles.
     */
    void handleAsteroidsCollisions();

    /**
     * @brief Broad phase collision detection, detects objects that could potentially collide with each other.
     */
    void broadPhase();

    /**
     * @brief Handles collisions between a player and every object.
     * @param player The player to handle collisions for.
     */
    void handleCollisionsNormalMavity(Player &player);

    /**
     * @brief Handles collisions between a player and every object when the mavity is reversed.
     * @param player The player to handle collisions for.
     */
    void handleCollisionsReversedMavity(Player &player) const;

    /**
     * @brief Main method that handle collisions for every player according to their mavity.
     * @see handleCollisionsNormalMavity() and handleCollisionsReversedMavity() for sub-functions.
     * TODO: add collision behavior for moving objects
     */
    void narrowPhase();

};

#endif //PLAY_TOGETHER_GAME_H
