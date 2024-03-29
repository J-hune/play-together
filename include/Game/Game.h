#ifndef PLAY_TOGETHER_GAME_H
#define PLAY_TOGETHER_GAME_H

#include <SDL.h>
#include <vector>
#include <climits>
#include <string>
#include <ranges>
#include <cmath>
#include "../Physics/CollisionHandler.h"

const float DISTANCE_OUT_MAP_BEFORE_DEATH = 500;

enum class GameState {
    RUNNING,
    PAUSED,
    STOPPED
};

/**
 * @file Game.h
 * @brief Defines the Game class responsible for handling the main game logic.
 */

/**
 * @class Game
 * @brief Represents the main game logic including initialization, event handling, collision detection, and rendering.
 */
class Game {
public:
    /** CONSTRUCTORS **/

    Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, const Player &initialPlayer);

    Game();


    /** ACCESSORS **/

    /**
     * @brief Returns the current game state.
     * @return The current game state.
     */
    [[nodiscard]] GameState getGameState() const;

    /**
     * @brief Returns the camera of the game.
     * @return A pointer of Camera object representing the camera of the game.
     */
    [[nodiscard]] Camera* getCamera();

    /**
     * @brief Get a point of the average position of all players combined.
     * @param[out] x The x-coordinate of the average players position
     * @param[out] y The y-coordinate of the average players position
     */
    [[nodiscard]] Point getAveragePlayersPositions() const;


    /** MODIFIERS **/

    /**
     * @brief Teleports the player to a specific location.
     * @param newX The X-coordinate of the location.
     * @param newY The Y-coordinate of the location.
     */
    void teleportPlayer(float newX, float newY);

    /**
     * @brief Set the level attribute.
     * @param map_name The name of the new map.
     */
    void setLevel(std::string const &map_name);

    /**
     * @brief Set a new state to render_camera_point
     * @param state the state of render_camera_point
     */
    void setRenderCameraPoint(bool state);

    /**
     * @brief Set a new state to render_camera_area
     * @param state the state of render_camera_area
     */
    void setRenderCameraArea(bool state);

    /**
     * @brief Set a new state to render_player_colliders
     * @param state the state of render_player_colliders
     */
    void setRenderPlayerColliders(bool state);

    /**
     * @brief Set a new state to enable_platforms_movement
     * @param state the state of enable_platforms_movement
     */
    void setEnablePlatformsMovement(bool state);

    /**
     * @brief Toggle the render_textures attribute, used for the application console.
     */
    void toggleRenderTextures();


    /** PUBLIC METHODS **/

    /**
     * @brief Runs the game loop.
     */
    void run();

    /**
     * @brief Pauses the game loop.
     */
    void pause();

    /**
     * @brief Stops the game loop and exits the game.
     */
    void stop();

    /**
     * @brief Adds a character to the game.
     * @param character The character to add.
     */
    void addCharacter(const Player &character);

    /**
     * @brief Removes a character from the game.
     * @param character The character to remove.
     */
    void removeCharacter(const Player &character);

    /**
     * @brief Serialize the game object
     * @tparam Archive
     * @param ar
     * @param version
     */
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & initialPlayer;
        ar & characters;
        ar & camera;
    }


private:
    /** ATTRIBUTES **/

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    Player initialPlayer; /**< The player object. */
    std::vector<Player> characters; /**< Collection of characters in the game. */
    bool isRunning = true; /**< Flag indicating if the game is running. */
    GameState gameState = GameState::STOPPED; /**< The current game state. */
    float deltaTime = 0; /**< The time elapsed. */
    static constexpr int fps = 60; /**< The game's fps limit. */

    // Debug variables used for the application console
    bool render_textures = true;
    bool render_camera_point = false;
    bool render_camera_area = false;
    bool render_player_colliders = false;
    bool enable_platforms_movement = true;


    /** PRIVATE METHODS **/

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     */
    void handleEvents(Player *player);

    /**
     * @brief Handles SDL Key up events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was release.
     */
    static void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent& keyEvent) ;

    /**
     * @brief Handles SDL Key down events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was press.
     */
    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent& keyEvent);

    /**
     * @brief Applies the movement to all players in the game.
     */
    void applyPlayersMovement();

    /**
     * @brief Calculates the movement of all players in the game.
     */
    void calculatePlayersMovement();

    /**
     * @brief Switch mavity between normal and reversed.
     */
    void switchMavity();

    /**
     * @brief Handles collisions between a player and every object.
     */
    void handleCollisionsNormalMavity(Player &player) const;

    /**
     * @brief Handles collisions between a player and every object when the mavity is reversed.
     */
    void handleCollisionsReversedMavity(Player &player) const;

    /**
     * @brief Main method that handle collisions for every player according to their mavity.
     * @see handleCollisionsNormalMavity() and handleCollisionsReversedMavity() for sub-functions.
     * TODO: add collision behavior for moving objects
     */
    void handleCollisions();

    /**
     * @brief Renders the game by drawing all the game textures and sprites.
     */
    void render();

};

#endif //PLAY_TOGETHER_GAME_H
