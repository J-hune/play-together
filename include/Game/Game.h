#ifndef PLAY_TOGETHER_GAME_H
#define PLAY_TOGETHER_GAME_H

#include <SDL.h>
#include <vector>
#include <climits>
#include <string>
#include <ranges>
#include <cmath>
#include <algorithm>
#include "Polygon.h"
#include "Camera.h"
#include "Level.h"
#include "Objects/Player.h"
#include "../Utils/Mediator.h"
#include "Objects/Asteroid.h"

const float DISTANCE_OUT_MAP_BEFORE_DEATH = 500;

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

    Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, bool *quitFlag);


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

    /**
     * @brief Get the characters vector.
     * @return The characters vector.
     */
    [[nodiscard]] std::vector<Player> &getCharacters();

    /**
     * @brief Get the level object.
     * @return The level object.
     */
    [[nodiscard]] Level &getLevel();

    /**
     * @brief Get the save slot.
     * @return The save slot.
     */
    [[nodiscard]] int getSaveSlot() const;


    /** MODIFIERS **/

    /**
     * @brief Find and return a player by its id.
     * @param id The id of the player to find (0 for the main player).
     * @return A pointer to the player object if found, nullptr otherwise.
     */
    Player* findPlayerById(int id);

    /**
     * @brief Find and return a player by its id.
     * @param id The id of the player to find (0 for the main player).
     * @return The index of the player object if found, -1 otherwise.
     */
    int findPlayerIndexById(int id);

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
     * @brief Set the save slot.
     * @param slot The save slot.
     */
    void setSaveSlot(int slot);

    /**
     * @brief Toggle the render_textures attribute, used for the application console.
     */
    void toggleRenderTextures();


    /** PUBLIC METHODS **/

    /**
     * @brief Initializes the game.
     * @param slot The save slot to use when saving the game.
     */
    void initialize(int slot=0);

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
     * @param characterPtr Pointer to the character to remove.
     */
    void removeCharacter(const Player* characterPtr);

    /**
    * @brief Handles SDL Key up events, updating the movement variables accordingly.
    * @param keyEvent Reference to the key who was release.
    */
    void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const;

    /**
     * @brief Handles SDL Key down events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was press.
     */
    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent& keyEvent);

    /**
     * @brief Saves the game state to a json file.
     */
    void saveGame();

private:
    /** ATTRIBUTES **/

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    std::vector<Asteroid> asteroids;
    std::vector<Player> characters; /**< Collection of characters in the game. */
    std::vector<Player> deadCharacters; /**< Collection of dead characters in the game. */
    bool *quitFlagPtr = nullptr; /**< Reference to the quit flag. */
    bool switchGravity = false; /**< Flag to indicate if the gravity should be switched. */
    int saveSlot = 0; /**< The save slot to use when saving the game. */

    // Debug variables used for the application console
    bool render_textures = true;
    bool render_camera_point = false;
    bool render_camera_area = false;
    bool render_player_colliders = false;
    bool enable_platforms_movement = true;

    GameState gameState = GameState::STOPPED; /**< The current game state. */


    /** PRIVATE METHODS **/

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     */
    void handleEvents();

    /**
     * @brief Applies player movement based on the current movement variables.
     * @param player The player to whom the movement will be applied.
     */
    void applyPlayerMovement(Player *player) const;

    /**
     * @brief Applies the movement to all players in the game.
     * @see applyPlayerMovement() for applying movement to one player.
     */
    void applyAllPlayerMovement();

    /**
     * @brief Calculate the movement of all players in the game.
     * @see Player::calculateMovement() for applying movement to one player.
     */
    void calculateAllPlayerMovement();

    /**
     * @brief Applies the movement to all asteroid in the game.
     * @see applyAsteroidMovement() for applying movement to one asteroid.
     */
    void applyAllAsteroidMovement();

    /**
     * @brief Updates the camera position based on the player's position.
     */
    void killPlayer(const Player *player);

    /**
     * @brief Checks for collision between the player and a polygon obstacle.
     * @param playerVertices The vector of Point representing the vertices of the player object.
     * @param obstacle The polygon obstacle.
     * @return True if a collision is detected, false otherwise.
     */
    static bool checkCollision(const std::vector<Point>& playerVertices, const Polygon &obstacle);

    /**
     * @brief Handles collisions between the player and obstacles.
     */
    void handleCollisionsWithObstacles(Player *player) const;

    /**
     * @brief Handles collisions between the player and death zones.
     */
    void handleCollisionsWithDeathZone(Player *player);

    /**
     * @brief Handles collisions between the player and save zones.
     */
    void handleCollisionsWithSaveZone(const Player *player);

    /**
     * @brief Handles collisions between the player and platforms.
     */
    void handleCollisionsWithPlatforms(Player *player) const;

    /**
     * @brief Handles collisions between the player and other players.
     */
    void handleCollisionsWithOtherPlayers(Player *player);

    /**
     * @brief Handles collisions between the player and camera borders.
     */
    void handleCollisionsWithCameraBorders(Player *player);

    /**
     * @brief Handles collisions between the asteroid and obstacles.
     */
    void handleCollisionsWithAsteroid();


    /**
     * @brief Generates a specified number of asteroids in the game.
     * @param nbAsteroid The number of asteroids must have in the game.
     */
    void generateAsteroid(int nbAsteroid);

    /**
     * @brief Handles collisions between the player and every object.
     * @see handleCollisionsReversedMavity() for handling collisions with reversed mavity.
     */
    void handleCollisions();

    /**
     * @brief Handles collisions between the player and platform when mavity is reversed.
     */
    void handleCollisionsWithObstaclesReverseMavity(Player *player);

    /**
     * @brief Handles collisions between the player and platforms when the mavity is reversed.
     */
    void handleCollisionsWithPlatformsReversedMavity(Player *player);

    /**
     * @brief Handles collisions between the player and other players when mavity is reversed.
     */
    void handleCollisionsWithCameraBordersReversedMavity(Player *player);

    /**
     * @brief Handles collisions between the player and camera borders when mavity is reversed.
     */
    void handleCollisionsWithOtherPlayersReversedMavity(Player *player);

    /**
     * @brief Main method that handle every collision when the mavity is reversed.
     * @see handleCollisions() for handling collisions with normal mavity.
     */
    void handleCollisionsReversedMavity();

    /**
     * @brief Renders the game by drawing all the game textures and sprites.
     */
    void render();


    /** STATIC METHODS **/

    /**
     * @brief Sends the keyboard state to the network.
     */
    static void sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMask);
};

#endif //PLAY_TOGETHER_GAME_H
