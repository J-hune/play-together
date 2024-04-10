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

static bool checkAABBCollisionTunnelingOnXAxis(Player *player, const SDL_FRect &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float move = player->getMoveX();
    bool direction = player->getDirectionX() == 1;

    SDL_FRect playerBoundingBox = {x + move, y, w, h};

    // Direction on x-axis is right
    if (direction) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move > 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            move -= w;
            playerBoundingBox = {x + move, y, w, h};
        }
    }
    // Direction on x-axis is left
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move < 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            move += w;
            playerBoundingBox = {x + move, y, w, h};
        }
    }

    // Check if a collision has been detected in the loop
    if ((direction && move > 0) || (!direction && move < 0)) {
        player->setMoveX(move); // Set moveX inside the collision
        return true;
    } else {
        return false;
    }
}

static bool checkAABBCollisionTunnelingOnYAxis(Player *player, const SDL_FRect &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float move = player->getMoveY();
    bool direction = player->getDirectionY() == 1;

    SDL_FRect playerBoundingBox = {x, y + move, w, h};

    // Direction on y-axis is down
    if (direction) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move > 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            move -= h;
            playerBoundingBox = {x, y + move, w, h};
        }
    }
    // Direction on y-axis is up
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move < 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            move += h;
            playerBoundingBox = {x, y + move, w, h};
        }
    }

    // Check if a collision has been detected in the loop
    if ((direction && move > 0) || (!direction && move < 0)) {
        player->setMoveY(move); // Set moveY inside the collision
        return true;
    } else {
        return false;
    }
}

static bool checkAABBCollisionTunnelingOnBothAxis(Player *player, const SDL_FRect &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float moveX = player->getMoveX();
    float moveY = player->getMoveY();
    bool directionX = player->getDirectionX() == 1; // True for right, false for left

    float addX; // Value that will be added to moveX at each iteration
    float addY; // Value that will be added to moveY at each iteration
    float add = w < h ? w : h; // Select the player's shortest side

    // Create a ratio between moveX and moveY so they increment proportionally
    float ratio = std::abs(moveX / moveY);
    if (ratio > 1) {
        addX = add;
        addY = add / ratio;
    }
    else {
        addX = add * ratio;
        addY = add;
    }

    // Choose whether to add or subtract the value according to direction
    if (directionX == 1) addX *= -1;
    if (player->getDirectionY() == 1) addY *= -1;

    SDL_FRect playerBoundingBox = {x + moveX, y + moveY, w, h};

    // The x-axis direction is right
    if (directionX) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (moveX > 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            moveX += addX;
            moveY += addY;
            playerBoundingBox = {x + moveX, y + moveY, w, h};
        }
    }
    // The x-axis direction is left
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (moveX < 0 && !checkAABBCollision(playerBoundingBox, obstacle)) {
            moveX += addX;
            moveY += addY;
            playerBoundingBox = {x + moveX, y + moveY, w, h};
        }
    }

    // Check if a collision has been detected in the loop
    if ((directionX && moveX > 0) || (!directionX && moveX < 0)) {
        // Set the move positions inside the collision
        player->setMoveX(moveX);
        player->setMoveY(moveY);
        return true;
    } else {
        return false;
    }
}

bool checkAABBCollisionTunneling(Player *player, const SDL_FRect &obstacle) {
    // The player has moved only on y-axis
    if (player->getMoveX() == 0 && player->getMoveY() != 0) {
        return checkAABBCollisionTunnelingOnYAxis(player, obstacle);
    }
    // The player has moved on both axis
    else if (player->getMoveX() != 0 && player->getMoveY() != 0) {
        return checkAABBCollisionTunnelingOnBothAxis(player, obstacle);
    }
    // The player has moved only on x-axis
    else if (player->getMoveX() != 0 && player->getMoveY() == 0) {
        return checkAABBCollisionTunnelingOnXAxis(player, obstacle);
    }
    // The player hasn't moved (supposed to be checked before calling this function)
    else {
        return false;
    }
}

static bool checkSATCollisionTunnelingOnXAxis(Player *player, const Polygon &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float move = player->getMoveX();
    bool direction = player->getDirectionX() == 1;

    std::vector<Point> playerVertices = {{x + move,     y},
                                         {x + move + w, y},
                                         {x + move + w, y + h},
                                         {x + move,     y + h}};

    // Direction on x-axis is right
    if (direction) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move > 0 && !checkSATCollision(playerVertices, obstacle)) {
            move -= w;
            playerVertices = {{x + move,     y},
                              {x + move + w, y},
                              {x + move + w, y + h},
                              {x + move,     y + h}};
        }
    }
    // Direction on x-axis is left
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move < 0 && !checkSATCollision(playerVertices, obstacle)) {
            move += w;
            playerVertices = {{x + move,     y},
                              {x + move + w, y},
                              {x + move + w, y + h},
                              {x + move,     y + h}};
        }
    }

    // Check if a collision has been detected in the loop
    if ((direction && move > 0) || (!direction && move < 0)) {
        player->setMoveX(move); // Set moveX inside the collision
        return true;
    } else {
        return false;
    }
}

static bool checkSATCollisionTunnelingOnYAxis(Player *player, const Polygon &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float move = player->getMoveY();
    bool direction = player->getDirectionY() == 1;

    std::vector<Point> playerVertices = {{x,     y + move},
                                         {x + w, y + move},
                                         {x + w, y + move + h},
                                         {x,     y + move + h}};

    // Direction on y-axis is down
    if (direction) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move > 0 && !checkSATCollision(playerVertices, obstacle)) {
            move -= h;
            playerVertices = {{x,     y + move},
                              {x + w, y + move},
                              {x + w, y + move + h},
                              {x,     y + move + h}};
        }
    }
    // Direction on y-axis is up
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (move < 0 && !checkSATCollision(playerVertices, obstacle)) {
            move += h;
            playerVertices = {{x,     y + move},
                              {x + w, y + move},
                              {x + w, y + move + h},
                              {x,     y + move + h}};
        }
    }

    // Check if a collision has been detected in the loop
    if ((direction && move > 0) || (!direction && move < 0)) {
        player->setMoveY(move); // Set moveY inside the collision
        return true;
    } else {
        return false;
    }
}


static bool checkSATCollisionTunnelingOnBothAxis(Player *player, const Polygon &obstacle) {

    // Initialization
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();
    float moveX = player->getMoveX();
    float moveY = player->getMoveY();
    bool directionX = player->getDirectionX() == 1; // True for right, false for left

    float addX; // Value that will be added to moveX at each iteration
    float addY; // Value that will be added to moveY at each iteration
    float add = w < h ? w : h; // Select the player's shortest side

    // Create a ratio between moveX and moveY so they increment proportionally
    float ratio = std::abs(moveX / moveY);
    if (ratio > 1) {
        addX = add;
        addY = add / ratio;
    }
    else {
        addX = add * ratio;
        addY = add;
    }

    // Choose whether to add or subtract the value according to direction
    if (directionX == 1) addX *= -1;
    if (player->getDirectionY() == 1) addY *= -1;

    std::vector<Point> playerVertices = {{x + moveX,     y + moveY},
                                         {x + moveX + w, y + moveY},
                                         {x + moveX + w, y + moveY + h},
                                         {x + moveX,     y + moveY + h}};

    // The x-axis direction is right
    if (directionX) {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (moveX > 0 && !checkSATCollision(playerVertices, obstacle)) {
            moveX += addX;
            moveY += addY;
            playerVertices = {{x + moveX,     y + moveY},
                              {x + moveX + w, y + moveY},
                              {x + moveX + w, y + moveY + h},
                              {x + moveX,     y + moveY + h}};
        }
    }
    // The x-axis direction is left
    else {
        // Check for collisions along the player's entire route to avoid tunneling effect
        while (moveX < 0 && !checkSATCollision(playerVertices, obstacle)) {
            moveX += addX;
            moveY += addY;
            playerVertices = {{x + moveX,     y + moveY},
                              {x + moveX + w, y + moveY},
                              {x + moveX + w, y + moveY + h},
                              {x + moveX,     y + moveY + h}};
        }
    }

    // Check if a collision has been detected in the loop
    if ((directionX && moveX > 0) || (!directionX && moveX < 0)) {
        // Set the move positions inside the collision
        player->setMoveX(moveX);
        player->setMoveY(moveY);
        return true;
    } else {
        return false;
    }
}

bool checkSATCollisionTunneling(Player *player, const Polygon &obstacle) {
    // The player has moved only on y-axis
    if (player->getMoveX() == 0 && player->getMoveY() != 0) {
        return checkSATCollisionTunnelingOnYAxis(player, obstacle);
    }
    // The player has moved on both axis
    else if (player->getMoveX() != 0 && player->getMoveY() != 0) {
        return checkSATCollisionTunnelingOnBothAxis(player, obstacle);
    }
    // The player has moved only on x-axis
    else if (player->getMoveX() != 0 && player->getMoveY() == 0) {
        return checkSATCollisionTunnelingOnXAxis(player, obstacle);
    }
    // The player hasn't moved (supposed to be checked before calling this function)
    else {
        return false;
    }
}

void correctAABBCollision(Player *player, const SDL_FRect &obstacle) {

    // INITIALIZATION
    float x = player->getX();
    float y = player->getY();
    float moveX = player->getMoveX();
    float moveY = player->getMoveY();

    bool xaxis = false;
    bool yaxis = false;


    // COLLISION ANALYSIS

    // Check if the x-axis movement is concerned by the collision
    SDL_FRect playerBoundingBox = {x, y - moveY, player->getW(), player->getH()};

    if (checkAABBCollision(playerBoundingBox, obstacle)) {
        xaxis = true;
    }

    // Check if the y-axis movement is concerned by the collision
    playerBoundingBox.x = x - moveX;
    playerBoundingBox.y = y;

    if (checkAABBCollision(playerBoundingBox, obstacle)) {
        yaxis = true;
    }


    // COLLISION CORRECTION

    // If the collision is on x-axis
    if (xaxis && !yaxis) {
        playerBoundingBox.x = x;

        if (player->getDirectionX() == -1) moveX = (obstacle.x + obstacle.w) - playerBoundingBox.x;
        else moveX = -((playerBoundingBox.x + playerBoundingBox.w) - obstacle.x);
        player->setX(player->getX() + moveX);
    }
    // If the collision is on y-axis
    else if (!xaxis && yaxis) {
        if (player->getDirectionY() == -1) moveY = (obstacle.y + obstacle.h) - playerBoundingBox.y;
        else moveY = -((playerBoundingBox.y + playerBoundingBox.h) - obstacle.y);
        player->setY(player->getY() + moveY);
    }
}

static float correctSATCollisionOnXAxis(const Player *player, const Polygon &obstacle) {

    // Initialize player positions
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();

    // Binary search variables initialization
    float move;
    float min = x - player->getMoveX();
    float max = x;

    std::vector<Point> playerVertices;

    int i = 0;

    // Reduce distance between the player and the polygon until the player is within 0.1 pixel of the polygon
    while ((std::abs(max - min) > 0.1F) && i < 32) {

        move = std::midpoint(max, min);

        playerVertices = {{move, y},
                          {move + w, y},
                          {move + w, y + h},
                          {move, y + h}};

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

static float correctSATCollisionOnYAxis(const Player *player, const Polygon &obstacle) {

    // Initialize player positions
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();

    // Binary search variables initialization
    float move;
    float min = y - player->getMoveY();
    float max = y;

    std::vector<Point> playerVertices;

    int i = 0;

    // Reduce distance between the player and the polygon until the player is within 0.1 pixel of the polygon
    while ((std::abs(max - min) > 0.1F) && i < 32) {

        move = std::midpoint(max, min);

        playerVertices = {{x, move},
                          {x + w, move},
                          {x + w, move + h},
                          {x, move + h}};

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

static void correctSATCollisionOnBothAxis(Player *player, const Polygon &obstacle) {

    // Initialize player positions
    float x = player->getX();
    float y = player->getY();
    float w = player->getW();
    float h = player->getH();

    // Binary search variables initialization
    float moveY;
    float divideY;
    float minY = y - 50;
    float maxY = y;

    float moveX;
    float divideX;
    float minX = x - 50;
    float maxX = x;

    // Create a ratio between moveX and moveY so they increment proportionally
    float ratio = std::abs(player->getMoveX() / player->getMoveY());
    if (ratio > 1) {
        divideX = 2;
        divideY = 2 / ratio;
    }
    else {
        divideX = 2 * ratio;
        divideY = 2;
    }

    std::vector<Point> playerVertices;

    int i = 0;

    // Reduce distance between the player and the polygon until the player is within 0.1 pixel of the polygon
    while ((std::abs(maxY - minY) > 0.1F) && i < 32) {

        moveX = std::lerp(minX, maxX, divideX);
        moveY = std::lerp(minY, maxY, divideY);

        playerVertices = {{moveX, moveY},
                          {moveX + w, moveY},
                          {moveX + w, moveY + h},
                          {moveX, moveY + h}};

        // The point is inside the collision
        if (checkSATCollision(playerVertices, obstacle)) {
            maxX = moveX;
            maxY = moveY;
        }
        // The point is outside the collision
        else {
            minX = moveX;
            minY = moveY;
        }

        i++;
    }

    player->setX(minX);
    player->setY(minY);
}

void correctSATCollision(Player *player, const Polygon &obstacle) {
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
    std::vector<Point> playerVertices = {{x, y - moveY},
                                         {x + w, y - moveY},
                                         {x + w, y - moveY + h},
                                         {x, y - moveY + h }};

    if (moveX != 0 && checkSATCollision(playerVertices, obstacle)) {
        xaxis = true;
    }

    // Check if the y-axis movement is concerned by the collision
    playerVertices = {{x - moveX, y},
                      {x - moveX + w, y},
                      {x - moveX + w, y + h},
                      {x - moveX, y + h}};

    if (moveY != 0 && checkSATCollision(playerVertices, obstacle)) {
        yaxis = true;
    }

    // COLLISION CORRECTION

    // If the collision is only on x-axis
    if (xaxis && !yaxis) {
        player->setX( correctSATCollisionOnXAxis(player, obstacle));
        player->setMoveX(0);
    }
    // If the collision is only on y-axis
    else if (!xaxis) {
        player->setY(correctSATCollisionOnYAxis(player, obstacle));
        player->setMoveY(0);
    }
    // If the collision is on both axis
    else {
        correctSATCollisionOnBothAxis(player, obstacle);
    }
}