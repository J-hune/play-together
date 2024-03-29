#include "../../include/Physics/CollisionManager.h"

/**
 * @file CollisionManager.cpp
 * @brief Implements functions for collision detection and correction.
 */


bool checkAABBCollision(const SDL_FRect &a, const SDL_FRect &b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

bool checkSATCollision(const std::vector<Point> &playerVertices, const Polygon &obstacle) {
    // Check for convexity of the obstacle
    if (!obstacle.isConvex()) {
        printf("The obstacle is not convex\n");
        return false;
    }

    // Potential separation axes (normals to the sides of the rectangle)
    std::vector<Point> axes = {
            {1, 0},
            {0, 1}
    };

    // Add axes perpendicular to the sides of the polygon
    for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
        std::vector<Point> vertices = obstacle.getVertices();
        const Point &vertex1 = vertices[i];
        const Point &vertex2 = vertices[(i + 1) % vertices.size()];

        Point edge = {vertex2.x - vertex1.x, vertex2.y - vertex1.y};
        Point axis = {-edge.y, edge.x};
        axes.push_back(axis);
    }

    for (Point axis: axes) {
        // Project the rectangle and the polygon onto the axis
        auto playerProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto playerProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: playerVertices) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            playerProjectionMin = std::min(playerProjectionMin, projection);
            playerProjectionMax = std::max(playerProjectionMax, projection);
        }

        auto obstacleProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto obstacleProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: obstacle.getVertices()) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            obstacleProjectionMin = std::min(obstacleProjectionMin, projection);
            obstacleProjectionMax = std::max(obstacleProjectionMax, projection);
        }

        // Check for separation on the axis
        if (playerProjectionMax < obstacleProjectionMin || obstacleProjectionMax < playerProjectionMin) {
            return false; // No collision detected
        }
    }

    return true; // Collision detected
}

void correctAABBCollisions(Player *player, const SDL_FRect &obstacle) {
    // INITIALIZATION

    float x = player->getX();
    float y = player->getY();
    float moveX = player->getMoveX();
    float moveY = player->getMoveY();

    bool xaxis = false;
    bool yaxis = false;


    // COLLISION ANALYSIS

    // Check if the x-axis movement is concerned by the collision
    SDL_FRect playerBoundingBox = {x + moveX, y, player->getW(), player->getH()};

    if (checkAABBCollision(playerBoundingBox, obstacle)) {
        xaxis = true;
    }

    // Check if the y-axis movement is concerned by the collision
    playerBoundingBox.x = x;
    playerBoundingBox.y = y + moveY;

    if (checkAABBCollision(playerBoundingBox, obstacle)) {
        yaxis = true;
    }

    // Add movement according to the analysis
    playerBoundingBox.x = xaxis ? x + moveX : x;
    playerBoundingBox.y = yaxis ? y + moveY : y;


    // COLLISION CORRECTION

    // If the collision is only on x-axis
    if (xaxis && !yaxis) {
        if (player->getDirectionX() == -1) moveX += (obstacle.x + obstacle.w) - playerBoundingBox.x;
        else moveX -= (playerBoundingBox.x + playerBoundingBox.w) - obstacle.x;
        player->setMoveX(moveX);
    }
        // If the collision is only on y-axis
    else if (!xaxis && yaxis) {
        if (player->getDirectionY() == -1) moveY += (obstacle.y + obstacle.h) - playerBoundingBox.y;
        else moveY -= (playerBoundingBox.y + playerBoundingBox.h) - obstacle.y;
        player->setMoveY(moveY);
    }
        // If the collisions is on the two axis (not supposed to happen)
    else {
        // Correct x-axis
        if (player->getDirectionX() == -1) moveX += (obstacle.x + obstacle.w) - playerBoundingBox.x;
        else moveX -= (playerBoundingBox.x + playerBoundingBox.w) - obstacle.x;
        player->setMoveX(moveX);

        // Correct y-axis
        if (player->getDirectionY() == -1) moveY += (obstacle.y + obstacle.h) - playerBoundingBox.y;
        else moveY -= (playerBoundingBox.y + playerBoundingBox.h) - obstacle.y;
        player->setMoveY(moveY);
    }
}

static float correctSATCollisionsOnXAxis(const Player *player, const Polygon &obstacle) {

    // Initialize player positions
    float x = player->getX();
    float y = player->getY() + player->getMoveY();
    float w = player->getW();
    float h = player->getH();

    // Binary search variables initialization
    float move;
    float min = 0;
    float max = player->getMoveX();

    std::vector<Point> playerVertices;

    int i = 0;

    // Reduce distance between the player and the polygon until the player is within 0.1 pixel of the polygon
    while ((std::abs(max - min) > 0.001F) && i < 32) {

        move = std::midpoint(max, min);

        playerVertices = {{x + move, y},
                          {x + move + w, y},
                          {x + move + w, y + h},
                          {x + move, y + h}};

        // The point is inside the collision
        if (checkSATCollision(playerVertices, obstacle)) {
            max = move;
        }
            // The point is outside the collision
        else {
            min = move;
        }

        i++;
    }

    return min;
}

static float correctSATCollisionsOnYAxis(const Player *player, const Polygon &obstacle) {

    // Initialize player positions
    float x = player->getX() + player->getMoveX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();

    // Binary search variables initialization
    float move;
    float min = 0;
    float max = player->getMoveY();

    std::vector<Point> playerVertices;

    int i = 0;

    // Reduce distance between the player and the polygon until the player is within 0.1 pixel of the polygon
    while ((std::abs(max - min) > 0.1F) && i < 32) {

        move = std::midpoint(max, min);

        playerVertices = {{x, y + move},
                          {x + w, y + move},
                          {x + w, y + move + h},
                          {x, y + move + h}};

        // The point is inside the collision
        if (checkSATCollision(playerVertices, obstacle)) {
            max = move;
        }
            // The point is outside the collision
        else {
            min = move;
        }

        i++;
    }

    return min;
}

void correctSATCollisions(Player *player, const Polygon &obstacle) {
    // INITIALIZATION

    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float moveX = player->getMoveX();
    float moveY = player->getMoveY();

    bool xaxis = false;
    bool yaxis = false;


    // COLLISION ANALYSIS

    // Check if the x-axis movement is concerned by the collision
    std::vector<Point> playerVertices = {{x + moveX, y},
                                         {x + moveX + w, y},
                                         {x + moveX + w, y + h},
                                         {x + moveX, y + h}};

    if (moveX != 0 && checkSATCollision(playerVertices, obstacle)) {
        xaxis = true;
    }

    // Check if the y-axis movement is concerned by the collision
    playerVertices = {{x, y + moveY},
                      {x + w, y + moveY},
                      {x + w, y + moveY + h},
                      {x, y + moveY + h}};

    if (moveY != 0 && checkSATCollision(playerVertices, obstacle)) {
        yaxis = true;
    }


    // COLLISION CORRECTION

    // If the collision is only on x-axis
    if (xaxis && !yaxis) {
        player->setMoveX(correctSATCollisionsOnXAxis(player, obstacle));
    }
        // If the collision is only on y-axis
    else if (!xaxis && yaxis) {
        player->setMoveY(correctSATCollisionsOnYAxis(player, obstacle));
    }
        // If the collisions is on the two axis (not supposed to happen)
    else {
        player->setMoveX(correctSATCollisionsOnXAxis(player, obstacle));
        player->setMoveY(correctSATCollisionsOnYAxis(player, obstacle));
    }
}