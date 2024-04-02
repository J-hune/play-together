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

    Game(SDL_Window *window, SDL_Renderer *renderer, int refreshRate, std::vector<TTF_Font *> &fonts, const Camera &camera, Level level, bool *quitFlag);


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
     * @brief Set the frame rate of the game.
     * @param frameRate The frame rate to set.
     */
    void setFrameRate(int frameRate);

    /**
     * @brief Toggle the render_textures attribute, used for the application console.
     */
    void toggleRenderTextures();

    /**
     * @brief Toggle the render_camera_point attribute, used for the application console.
     */
    void toggleRenderFps();


    /** PUBLIC METHODS **/

    /**
     * @brief Initializes the game.
     * @param slot The save slot to use when saving the game.
     */
    void initialize(int slot=0);

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
     * @brief Updates the camera position based on the player's position.
     * @param player The player to kill.
     */
    void killPlayer(const Player *player);

    /**
     * @brief Removes a character from the game.
     * @param characterPtr Pointer to the character to remove.
     */
    void removeCharacter(const Player *characterPtr);

    /**
    * @brief Handles SDL Key up events, updating the movement variables accordingly.
    * @param keyEvent Reference to the key who was release.
    */
    static void handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) ;

    /**
     * @brief Handles SDL Key down events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was press.
     * @param keyEvent Reference to the key who was press.
     */
    void handleKeyDownEvent(Player *player, const SDL_KeyboardEvent& keyEvent);

    /**
     * @brief Saves the game state to a json file.
     */
    void saveGame() const;

private:
    /** ATTRIBUTES **/

    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    int frameRate = 60; /**< The refresh rate of the game. */
    const int tickRate = 30; /**< The tick rate of the game. */
    std::vector<TTF_Font *> &fonts; /**< TTF fonts for rendering text. */
    Camera camera; /**< The camera object */
    Level level; /**< The level object */
    std::vector<Player> characters; /**< Collection of characters in the game. */
    std::vector<Player> deadCharacters; /**< Collection of dead characters in the game. */
    bool *quitFlagPtr = nullptr; /**< Reference to the quit flag. */
    int saveSlot = 0; /**< The save slot to use when saving the game. */
    int effectiveFrameFps = frameRate; /**< The effective fps. */
    GameState gameState = GameState::STOPPED; /**< The current game state. */

    // Broad phase attributes
    std::vector<Polygon> saveZones; /**< Collection of polygons representing save zones. */
    std::vector<Polygon> deathZones; /**< Collection of polygons representing death zones. */
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    std::vector<MovingPlatform1D> movingPlatforms1D; /**< Collection of MovingPlatform1D representing 1D platforms. */
    std::vector<MovingPlatform2D> movingPlatforms2D; /**< Collection of MovingPlatform2D representing 2D platforms. */
    std::vector<SwitchingPlatform> switchingPlatforms; /**< Collection of switchingPlatform representing switching platforms. */

    // Debug variables used for the application console
    bool render_textures = true;
    bool render_camera_point = false;
    bool render_camera_area = false;
    bool render_player_colliders = false;
    bool render_fps = false;
    bool enable_platforms_movement = true;


    /** PRIVATE METHODS **/

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     */
    void handleEvents();

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
     * @brief Switch mavity between normal and reversed.
     */
    void switchMavity();

    /**
     * @brief Broad phase collision detection, detects objects that could potentially collide with each other.
     */
    void broadPhase();

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
    void narrowPhase();

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
