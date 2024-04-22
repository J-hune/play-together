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
#include "../Utils/Mediator.h"
#include "../Utils/MessageQueue.h"
#include "Level.h"
#include "GameManagers/PlayerCollisionManager.h"
#include "GameManagers/InputManager.h"
#include "GameManagers/TextureManager.h"
#include "GameManagers/RenderManager.h"
#include "GameManagers/SaveManager.h"
#include "GameManagers/PlayerManager.h"
#include "GameManagers/BroadPhaseManager.h"
#include "GameManagers/EventCollisionManager.h"


/**
 * @file Game.h
 * @brief Defines the Game class responsible for handling the main game logic.
 */

// Forward declaration of managers
class InputManager;
class RenderManager;
class SaveManager;
class BroadPhaseManager;
class PlayerManager;
class PlayerCollisionManager;
class EventCollisionManager;


/**
 * @class Game
 * @brief Represents the main game logic including initialization, event handling, collision detection, and rendering.
 */
class Game {
private:
    /* ATTRIBUTES */
    static constexpr float effectiveFrameRateUpdateIntervalSeconds = 1.0f;
    static constexpr float networkInputSendIntervalSeconds = 1.0f / 60.0f;
    static constexpr float networkSyncCorrectionIntervalSeconds = 0.50f;

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */

    std::unique_ptr<InputManager> inputManager; /**< Input manager for handling input events. */
    std::unique_ptr<TextureManager> textureManager; /**< Texture manager for handling texture loading. */
    std::unique_ptr<RenderManager> renderManager; /**< Renderer object for rendering the game. */
    std::unique_ptr<SaveManager> saveManager; /**< Save manager for saving and loading the game state. */
    std::unique_ptr<BroadPhaseManager> broadPhaseManager; /**< Broad phase manager for handling the collision broad phase in the game. */
    std::unique_ptr<PlayerManager> playerManager; /**< Player manager for handling the players in the game. */
    std::unique_ptr<PlayerCollisionManager> playerCollisionManager; /**< Player collision manager for handling the player collisions in the game. */
    std::unique_ptr<EventCollisionManager> eventCollisionManager; /**< Event collision manager for handling the event collisions in the game. */

    int frameRate = 60; /**< The refresh rate of the game. */
    int effectiveFrameFps = frameRate; /**< The effective fps. */
    Uint32 lastPlaytimeUpdate = SDL_GetTicks(); /**< The last time that playtime was updated. */
    Uint32 playtime = 0; /**< The time in milliseconds elapsed since the game started. */

    GameState gameState = GameState::STOPPED; /**< The current game state. */
    bool *quitFlagPtr = nullptr; /**< Reference to the quit flag. */
    MessageQueue *messageQueue; /**< The message queue for communication between threads. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    Music music; /**< Represents the music that is currently played in the game. */
    size_t seed;


public:
    /* CONSTRUCTORS */

    Game(SDL_Window *window, SDL_Renderer *renderer, int frameRate, bool *quitFlag, MessageQueue *messageQueue);


    /* ACCESSORS */

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
     * @brief Returns the texture manager of the game.
     * @return A pointer of TextureManager representing the render manager of the game.
     */
    [[nodiscard]] TextureManager &getTextureManager();

    /**
     * @brief Returns the render manager of the game.
     * @return A pointer of RenderManager representing the render manager of the game.
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
     * @brief Returns the broad phase manager of the game.
     * @return A pointer of BroadPhaseManager object representing the broad phase manager of the game.
     */
    [[nodiscard]] BroadPhaseManager &getBroadPhaseManager();

    /**
     * @brief Returns the camera of the game.
     * @return A pointer of Camera object representing the camera of the game.
     */
    [[nodiscard]] Camera* getCamera();

    /**
     * @brief Get the level object.
     * @return The level object.
     */
    [[nodiscard]] Level* getLevel();

    /**
     * @brief Get the frame rate of the game.
     * @return The frame rate of the game.
     */
    [[nodiscard]] int getEffectiveFrameRate() const;

    /**
     * @brief Update and get the playtime of the game in milliseconds.
     * @return The playtime of the game.
     */
    [[nodiscard]] Uint32 getPlaytime();


    /* MODIFIERS */

    /**
     * @brief Set the level attribute.
     * @param map_name The name of the new map.
     */
    void setLevel(std::string const &map_name);

    /**
     * @brief Set the frame rate of the game.
     * @param fps The frame rate to set.
     */
    void setFrameRate(int fps);

    /**
     * @brief Set the playtime of the game.
     * @param new_playtime The playtime to set.
     */
    void setPlaytime(Uint32 new_playtime);

    /**
     * @brief Switch mavity between normal and reversed.
     */
    void switchMavity();


    /* PUBLIC METHODS */

    /**
     * @brief Initializes the game by loading the level and the character.
     * @param slot The save slot to use when loading the game. (0 by default)
     */
    void initializeHostedGame(int slot = 0);

    /**
     * @brief Initializes a new game by loading the level data received from the server.
     * @param map_name The name of the map to load.
     * @param last_checkpoint The last checkpoint reached.
     * @param players The list of players to load.
     */
    void loadLevel(const std::string &map_name, short last_checkpoint, const nlohmann::json::array_t &players,
                   const nlohmann::json::array_t &movingPlatforms1D, const nlohmann::json::array_t &movingPlatforms2D, const nlohmann::json::array_t &crushers);

    /**
     * @brief Updates the game logic.
     * @param delta_time The time elapsed since the last frame in seconds.
     */
    void update(double delta_time);

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

    /* PRIVATE METHODS */

    /**
     * @brief Applies the movement to all players in the game.
     * @param delta_time The time elapsed since the last frame in seconds.
     */
    void applyPlayersMovement(double delta_time);

    /**
     * @brief Calculates the movement of all players in the game.
     * @param delta_time The time elapsed since the last frame in seconds.
     */
    void calculatePlayersMovement(double delta_time);

    /**
     * @brief Main method that handle collisions for every player according to their mavity.
     * @param delta_time The time elapsed since the last frame in seconds.
     * @see handleCollisionsNormalMavity() and handleCollisionsReversedMavity() for sub-functions.
     */
    void narrowPhase(double delta_time);

};

#endif //PLAY_TOGETHER_GAME_H
