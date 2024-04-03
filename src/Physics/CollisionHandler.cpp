#include "../../include/Physics/CollisionHandler.h"

/**
 * @file CollisionHandler.cpp
 * @brief Implements functions for handling collisions behavior.
 */

/* COLLISIONS RELATED TO MAVITY */
/* NORMAL MAVITY */

void handleCollisionsWithObstacles(Player *player, const std::vector<Polygon> &obstacles) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: obstacles) {
        // Check if a collision is detected
        if (checkSATCollisionTunneling(player, obstacle)) {

            correctSATCollision(player, obstacle); // Correct the collision

            // If the collision is with the ground, the player is on a platform
            if (!player->getIsOnPlatform() && checkSATCollision(player->getGroundColliderVertices(), obstacle)) {
                player->setIsOnPlatform(true);
            }
            // If the collision is with the wall, the player can't move
            if (player->getCanMove() && checkSATCollision(player->getHorizontalColliderVertices(), obstacle)) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsWithMovingPlatform1D(Player *player, const std::vector<MovingPlatform1D> &platforms) {
    // Check for collisions with each 1D moving platforms
    for (const MovingPlatform1D &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform x-axis velocity to the player
                if (!platform.getAxis()) player->setMoveX(player->getMoveX() + platform.getMove());
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsWithMovingPlatform2D(Player *player, const std::vector<MovingPlatform2D> &platforms) {

    // Check for collisions with each 2D moving platforms
    for (const MovingPlatform2D &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                player->setX(player->getX() + platform.getMoveX());
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsWithSwitchingPlatform(Player *player, const std::vector<SwitchingPlatform> &platforms) {

    // Check for collisions with each switching platforms
    for (const SwitchingPlatform &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}

/*
void handleCollisionsWithPlayers(Player *player, const std::vector<Player>& players) {
    // Get others players in a vector
    std::vector<Player*> otherCharacters;
    if (*player != initialPlayer) otherCharacters.push_back(&initialPlayer);
    for (Player &character : characters) {
        if (*player != character) otherCharacters.push_back(&character);
    }

    // Check for collisions with other characters
    for (Player *character : otherCharacters) {
        if (checkAABBCollision(player->getBoundingBox(), character->getBoundingBox())) {

            //The player move to the right
            if (player->getMoveX() > 0) {
                //  The player's right is to the right of the detected player's left
                if (player->getX()+player->getW() <= character->getX() + 2) {
                    // Slow down the player's speed
                    player->setMoveX(player->getMoveX() / 5);
                    character->setMoveX(player->getMoveX());
                }
            }
                //The player move to the left
            else if (player->getMoveX() < 0) {
                if (player->getX() + 2 > character->getX()+character->getW()) {
                    player->setMoveX(player->getMoveX() / 5);
                    character->setMoveX(player->getMoveX());
                }
            }

            // If above the detected player
            if (!player->getIsOnPlatform()
                //The player's bottom is below the detected player's head
                && player->getY()+player->getH()>character->getY()
                // The player's head is above the detected player's head
                && player->getY()<character->getY()
                // The player's body is well within the x range of the detected player's body
                && player->getX()+player->getW()>character->getX()+2 // The player's right is to the right of the detected player's left
                && player->getX()+2<character->getX()+character->getW()) { // The player's left is to the left of the detected player's right
                player->setIsOnPlatform(true);
            }

            // If below the detected player
            //The player's head is above the detected player's bottom
            if (player->getY()<character->getY()+character->getH() &&
                // The player's head is below the detected player's head
                player->getY() > character->getY()
                // The player's body is well within the x range of the detected player's body
                && player->getX()+player->getW()>character->getX()+2 // The player's right is to the right of the detected player's left
                && player->getX()+2<character->getX()+character->getW()) { // The player's left is to the left of the detected player's right
                player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
            }

        }
    }
}

void handleCollisionsWithCameraBorders(Player *player) {
    // Get others players in a vector
    std::vector<Player> otherCharacters;
    if (*player != initialPlayer) otherCharacters.push_back(initialPlayer);
    for (const Player &character : characters) {
        if (*player != character) otherCharacters.push_back(character);
    }

    // Check for collisions with down camera borders
    if (player->getY() > camera.getY() + camera.getH() - player->getH() + DISTANCE_OUT_MAP_BEFORE_DEATH){
        printf("WASTED\n");

        //Temporarily resets the player to x=50 and y=50 being the player's spawn points.
        player->setX(50);
        player->setY(50);
    }

    // Check for collisions with up camera borders
    if (player->getY() < camera.getY() - DISTANCE_OUT_MAP_BEFORE_DEATH){
        printf("WASTED\n");

        //Temporarily resets the player to x=50 and y=50 being the player's spawn points.
        player->setX(camera.getX());
        player->setY(camera.getY());
    }


    // Check for collisions with right camera borders
    if (player->getX() > camera.getX() + camera.getW() - player->getW()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX() / 5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the left camera borders
        for (Player &character : otherCharacters){
            if(character.getX() < camera.getX()){
                character.setMoveX(player->getMoveX());
            }
        }

    }
        // Check for collisions with left camera borders
    else if (player->getX() < camera.getX()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX() / 5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the right camera borders
        for (Player &character: otherCharacters) {
            if (character.getX() > camera.getX() + camera.getW() - character.getW()) {
                character.setX(player->getMoveX());
            }
        }
    }
}

*/

bool handleCollisionsWithDeathZones(const Player &player, std::vector<Polygon> &deathZones) {
    size_t i = 0;

    // Check collisions with each death zone until the player is dead
    while (i < deathZones.size() && !checkSATCollision(player.getVertices(), deathZones[i])) {
        i++;
    }

    return i != deathZones.size();
}

void handleCollisionsWithSaveZones(const Player &player, Level &level, std::vector<Polygon> &saveZones) {
    // Check collisions with each save zone
    for (size_t i = 0; i < saveZones.size(); ++i) {
        const Polygon &saveZone = saveZones[i];        // If collision detected with the save zone, save the player (in local or server mode)
        auto savePointID = static_cast<short>(i + 1);
        if (checkSATCollision(player.getVertices(), saveZone) && level.getLastCheckpoint() < savePointID) {
            level.setLastCheckpoint(savePointID);
            std::cout << "Checkpoint reached: " << savePointID << std::endl;
        }
    }
}


/* REVERSED MAVITY */

void handleCollisionsSelcatsbOhtiw(Player *player, const std::vector<Polygon> &obstacles) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: obstacles) {
        // Check if a collision is detected
        if (checkSATCollisionTunneling(player, obstacle)) {

            correctSATCollision(player, obstacle); // Correct the collision

            // If the collision is with the roof, the player is on a platform
            if (checkSATCollision(player->getRoofColliderVertices(), obstacle)) {
                player->setIsOnPlatform(true);
            }
            // If the collision is with the wall, the player can't move
            if (player->getCanMove() && checkSATCollision(player->getHorizontalColliderVertices(), obstacle)) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsD1mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform1D> &platforms) {
    // Check for collisions with each 1D moving platforms
    for (const MovingPlatform1D &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If collision detected with the roof, the player is on the platform
            if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform x-axis velocity to the player
                if (!platform.getAxis()) player->setMoveX(player->getMoveX() + platform.getMove());
            }
            // If collision detected with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsD2mroftalPgnivoMhtiw(Player *player, const std::vector<MovingPlatform2D> &platforms) {
    // Check for collisions with each 2D moving platforms
    for (const MovingPlatform2D &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player is on a platform
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                player->setX(player->getX() + platform.getMoveX());
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}

void handleCollisionsMroftalPgnihctiwShtiw(Player *player, const std::vector<SwitchingPlatform> &platforms) {
    // Check for collisions with each switching platforms
    for (const SwitchingPlatform &platform: platforms) {
        // Check if a collision is detected
        if (checkAABBCollisionTunneling(player, platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player is on a platform
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->setCanMove(false);
            }
        }
    }
}
