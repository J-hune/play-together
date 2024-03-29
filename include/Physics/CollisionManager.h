#ifndef PLAY_TOGETHER_COLLISIONMANAGER_H
#define PLAY_TOGETHER_COLLISIONMANAGER_H

#include <SDL.h>
#include <numeric>
#include "Polygon.h"
#include "../Game/Objects/Player.h"

/**
 * @file CollisionManager.h
 * @brief Declaration of functions for collision detection and correction.
 */


/**
 * @brief Check for Axis-Aligned Bounding Box (AABB) collision between two rectangles.
 * @param a SDL_FRect representing the first rectangle.
 * @param b SDL_FRect representing the second rectangle.
 * @return True if collision detected, false otherwise.
 */
bool checkAABBCollision(const SDL_FRect &a, const SDL_FRect &b);

/**
 * @brief Checks for Separating Axes Theorem (SAT) collision between two polygons.
 * @param playerVertices The vector of Point representing the vertices of the player object.
 * @param obstacle The polygon obstacle.
 * @return True if a collision is detected, false otherwise.
 */
bool checkSATCollision(const std::vector<Point> &playerVertices, const Polygon &obstacle);

/**
 * @brief Corrects an AABB collision between a player and a fixed obstacle by moving the player.
 * @param player The player object that collided with the obstacle.
 * @param obstacle A SDL_FRect representing the fixed obstacle with which the player collided.
 * @pre The two objects must collide.
 * @see checkAABBCollision() to detect AABB collisions.
 */
void correctAABBCollisions(Player *player, const SDL_FRect &obstacle);

/**
 * @brief Corrects a SAT collision between a player and an obstacle by moving the player.
 * @param playerVertices The vector of Point representing the vertices of the player object.
 * @param obstacle The polygon obstacle.
 * @pre The two objects must collide.
 * @see correctSATCollisionsOnXAxis() and correctSATCollisionsOnYAxis() for sub-functions.
 */
void correctSATCollisions(Player *player, const Polygon &obstacle);

#endif //PLAY_TOGETHER_COLLISIONMANAGER_H
