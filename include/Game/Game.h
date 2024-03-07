#ifndef PLAY_TOGETHER_GAME_H
#define PLAY_TOGETHER_GAME_H

#include <SDL.h>
#include <vector>
#include <climits>
#include <string>
#include <ranges>
#include "Polygon.h"
#include "Player.h"
#include "Camera.h"
#include "Level.h"

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

    Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level,
         const Player &initialPlayer);

    Game();


    /** ACCESSORS **/

    /**
     * @brief Returns the current game state.
     * @return The current game state.
     */
    [[nodiscard]] GameState getGameState() const;

    /**
     * @brief Get a point of the average position of all players combined.
     * @param[out] x The x-coordinate of the average players position
     * @param[out] y The y-coordinate of the average players position
     */
    [[nodiscard]] Point getAveragePlayersPositions() const;


    /** MODIFIERS **/

    /**
     * @brief Set the level attribute.
     * @param map_name The name of the new map.
     */
    void setLevel(std::string const &map_name);

    /**
     * @brief Sets the shaking state of the camera.
     * @param val The new value for the shaking state of the camera.
     */
    void setCameraIsShaking(bool state);

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
     * @brief Teleports the player to a specific location.
     * @param x The X-coordinate of the location.
     * @param y The Y-coordinate of the location.
     */
    void teleportPlayer(float x, float y);

    /**
     * @brief Serialize the game object
     * @tparam Archive
     * @param ar
     * @param version
     */
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & player;
        ar & characters;
        ar & camera;
    }


private:
    /** ATTRIBUTES **/

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    Player player; /**< The player object. */
    std::vector<Player> characters; /**< Collection of characters in the game. */
    bool render_camera_point = false;
    bool render_camera_area = false;

    GameState gameState = GameState::STOPPED; /**< The current game state. */


    /** PRIVATE METHODS **/

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     * @param moveX Reference to the X-axis movement variable.
     * @param moveY Reference to the Y-axis movement variable.
     */
    void handleEvents(float &moveX, float &moveY);

    /**
     * @brief Applies player movement based on the current movement variables.
     * @param moveX The movement along the X-axis.
     * @param moveY The movement along the Y-axis.
     */
    void applyPlayerMovement(float moveX, float moveY);

    /**
     * @brief Checks for collision between the player and a polygon obstacle.
     * @param player The player object.
     * @param obstacle The polygon obstacle.
     * @return True if a collision is detected, false otherwise.
     */
    static bool checkCollision(const Player &player, const Polygon &obstacle);

    /**
     * @brief Handles collisions between the player and obstacles.
     * @param moveX The movement along the X-axis.
     * @param moveY The movement along the Y-axis.
     */
    void handleCollisions(float moveX, float moveY);

    /**
     * @brief Renders the game by drawing the player and obstacles.
     */
    void render();

};

#endif //PLAY_TOGETHER_GAME_H
