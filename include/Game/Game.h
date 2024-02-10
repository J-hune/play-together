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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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
    SDL_Rect camera;
    bool isRunning = true; /**< Flag indicating if the game is running. */

    /**
     * @brief Handles SDL events, updating the movement variables accordingly.
     * @param moveX Reference to the X-axis movement variable.
     * @param moveY Reference to the Y-axis movement variable.
     */
    void handleEvents(int &moveX, int &moveY);

    /**
     * @brief Applies player movement based on the current movement variables.
     * @param moveX The movement along the X-axis.
     * @param moveY The movement along the Y-axis.
     */
    void applyPlayerMovement(int moveX, int moveY);

    /**
     * @brief Applies camera movement based on the positions of all players.
     */
    void applyCameraMovement();

    /**
     * @brief Handles collisions between the player and obstacles.
     * @param moveX The movement along the X-axis.
     * @param moveY The movement along the Y-axis.
     */
    void handleCollisions(int moveX, int moveY);

    /**
     * @brief Renders the game by drawing the player and obstacles.
     */
    void render();

    /**
     * @brief Checks for collision between the player and a polygon obstacle.
     * @param player The player object.
     * @param obstacle The polygon obstacle.
     * @return True if a collision is detected, false otherwise.
     */
    static bool checkCollision(const Player &player, const Polygon &obstacle);

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
    void teleportPlayer(int x, int y);
};

#endif //PLAY_TOGETHER_GAME_H
