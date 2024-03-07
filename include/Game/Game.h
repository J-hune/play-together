#ifndef PLAY_TOGETHER_GAME_H
#define PLAY_TOGETHER_GAME_H

#include <vector>
#include <SDL.h>
#include "Polygon.h"
#include "Player.h"
#include <climits>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <cmath>

constexpr float SCREEN_WIDTH = 800;
constexpr float SCREEN_HEIGHT = 600;
constexpr float LERP_SMOOTHING_FACTOR = 0.05f;

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
    Game(SDL_Window* window, SDL_Renderer* renderer, const Player& initialPlayer); /**< Constructor for the Game class. */

    /**
    * @brief Initialize the camera position according to players positions.
    */
    void initializeCameraPosition();

    /**
     * @brief Runs the game loop.
     */
    void run();

    /**
     * @brief Stops the game loop and cleans up resources.
     */
    void stop();

/**
 * @brief Loads obstacles from a map file.
 * @param mapName The name of the map file.
 */
void loadPolygonsFromMap(const std::string &mapName);

private:
    SDL_Window *window; /**< SDL window for rendering. */
    SDL_Renderer *renderer; /**< SDL renderer for rendering graphics. */
    std::vector<Polygon> obstacles; /**< Collection of polygons representing obstacles. */
    Player player; /**< The player object. */
    std::vector<Player> characters; /**< Collection of characters in the game. */
    SDL_FRect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; /**< Rectangle for camera object */
    bool isRunning = true; /**< Flag indicating if the game is running. */
    bool switchGravity = false;

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     * @param direction Reference to the direction of the player.
     * @param moveY Reference to the Y-axis movement variable.
     */
    void handleEvents(int &direction, float &moveY);

    /**
     * @brief Handles SDL Key up events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was release.
     */
    void handleKeyUpEvent(const SDL_KeyboardEvent& keyEvent);

    /**
     * @brief Handles SDL Key down events, updating the movement variables accordingly.
     * @param keyEvent Reference to the key who was press.
     * @param direction Reference to the direction of the player.
     * @param moveY Reference to the Y-axis movement variable.
     */
    void handleKeyDownEvent(const SDL_KeyboardEvent& keyEvent, int &direction, float &moveY);

    /**
     * @brief Applies player movement based on the current movement variables.
     * @param moveX The movement along the X-axis.
     * @param direction The direction along the X-axis.
     * @param timeSpeed The time that has elapsed since the player started running.
     * @param moveY The movement along the Y-axis.
     */
    void applyPlayerMovement(float &moveX, int direction, float &moveY);

    /**
     * @brief Get a point of the average position of all players combined.
     * @param[out] x The x-coordinate of the average players position
     * @param[out] y The y-coordinate of the average players position
     */
    void getAveragePlayersPositions(float *x, float *y) const;

    /**
     * @brief Applies camera movement based on the positions of all players.
     */
    void applyCameraMovement();

    /**
     * @brief Handles collisions between the player and obstacles.
     * @param direction The direction along the X-axis.
     * @param moveY The movement along the Y-axis.
     */
    void handleCollisions(int direction, float moveY, float &moveX);

    /**
     * @brief Renders the game by drawing the player and obstacles.
     */
    void render();

    /**
     * @brief Checks for collision between the player and a polygon obstacle.
     * @param playerVertices The vector of Point representing the vertices of the player object.
     * @param obstacle The polygon obstacle.
     * @return True if a collision is detected, false otherwise.
     */
    static bool checkCollision(const std::vector<Point>& playerVertices, const Polygon &obstacle);

    /**
     * @brief Checks if a polygon is convex.
     * @param polygon The polygon to check.
     * @return True if the polygon is convex, false otherwise.
     */
    static bool isConvex(const Polygon &polygon);

public:
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
};

#endif //PLAY_TOGETHER_GAME_H
