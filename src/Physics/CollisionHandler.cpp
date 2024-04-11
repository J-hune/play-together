#include "../../include/Physics/CollisionHandler.h"

/**
 * @file CollisionHandler.cpp
 * @brief Implements functions for handling collisions behavior.
 */

/* PLAYER NORMAL MAVITY */

void handleCollisionsWithObstacles(Player *player, const std::vector<Polygon> &obstacles) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: obstacles) {
        // Check if a collision is detected
        if (checkSATCollision(player->getVertices(), obstacle)) {

            correctSATCollision(player, obstacle); // Correct the collision

            // If the collision is with the roof, the player is can't jump anymore
            if (checkSATCollision(player->getRoofColliderVertices(), obstacle)) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player is can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                if (!platform.getAxis()) player->setX(player->getX() + platform.getMove());
                else player->setY(player->getY() + platform.getMove());
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player is can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                player->setX(player->getX() + platform.getMoveX());
                player->setY(player->getY() + platform.getMoveY());
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player is can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
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
void handleCollisionsWithPlayers(Player *player, const std::vector<Player> &players) {
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
}*/

bool handleCollisionsWithCameraBorders(const SDL_FRect player, const SDL_FRect camera) {
    return player.x < camera.x - DISTANCE_OUT_MAP_BEFORE_DEATH
            || player.x + player.w > camera.x + camera.w + DISTANCE_OUT_MAP_BEFORE_DEATH
            || player.y < camera.y - DISTANCE_OUT_MAP_BEFORE_DEATH
            || player.y > camera.y + camera.h + DISTANCE_OUT_MAP_BEFORE_DEATH;
}

bool handleCollisionsWithDeathZones(const Player &player, std::vector<Polygon> &deathZones) {
    size_t i = 0;

    // Check collisions with each death zone until the player is dead
    while (i < deathZones.size() && !checkSATCollision(player.getVertices(), deathZones[i])) {
        i++;
    }

    return i != deathZones.size(); // True if the player collided with a death zone
}

void handleCollisionsWithSaveZones(Player &player, Level &level, std::vector<AABB> &saveZones) {
    // Check collisions with each save zone
    for (size_t i = 0; i < saveZones.size(); ++i) {
        const AABB &saveZone = saveZones[i];        // If collision detected with the save zone, save the player (in local or server mode)
        auto savePointID = static_cast<short>(i + 1);

        // Check if the player is already in the save zone
        if (player.getCurrentZoneID() == (i + 1)) return;
        if (checkAABBCollision(player.getBoundingBox(), saveZone.getRect()) && level.getLastCheckpoint() < savePointID) {
            level.setLastCheckpoint(savePointID);
            player.setCurrentZoneID(i + 1);
            std::cout << "Checkpoint reached: " << savePointID << std::endl;
        }
    }
}

void handleCollisionsWithToggleGravityZones(Player &player, const std::vector<AABB> &toggleGravityZones) {
    // Check collisions with each toggle gravity zone
    for (size_t i = 0; i < toggleGravityZones.size(); ++i) {
        const AABB &toggleGravityZone = toggleGravityZones[i];
        const SDL_FRect &playerBoundingBox = player.getBoundingBox();
        const SDL_FRect &toggleGravityRect = toggleGravityZone.getRect();

        if (checkAABBCollision(playerBoundingBox, toggleGravityRect)) {
            const size_t zoneID = i + 100;

            // If the player was already in the zone, skip the zone
            if (player.getCurrentZoneID() == zoneID) {
                continue;
            }

            player.toggleMavity();
            player.getSprite()->toggleFlipVertical();

            float gravityOffset = toggleGravityZone.getHeight() / 5;
            float currentMoveY = player.getMoveY();
            float newMoveY = (player.getMavity() < 0) ? std::max(currentMoveY, gravityOffset) : std::min(currentMoveY, -gravityOffset);
            player.setMoveY(newMoveY);
            player.setCurrentZoneID(zoneID);

            // Exit the loop if the player is in the zone (no need to check the other zones)
            return;
        }

        // If the player is in the zone, but not colliding with it anymore, reset the player's zone
        else if (player.getCurrentZoneID() == i + 100) {
            player.setCurrentZoneID(0);
        }
    }
}

void handleCollisionsWithIncreaseFallSpeedZones(Player &player, const std::vector<AABB> &increaseFallSpeedZones) {
    // Check collisions with each increase fall speed zone
    for (const auto & increaseFallSpeedZone : increaseFallSpeedZones) {
        // Check if the player is already in the increase fall speed zone
        if (checkAABBCollision(player.getBoundingBox(), increaseFallSpeedZone.getRect())) {
            player.setMaxFallSpeed(1300);
        } else {
            player.setCurrentZoneID(0);
            player.setMaxFallSpeed(600);
        }
    }
}

/* PLAYER REVERSED MAVITY */

void handleCollisionsSelcatsbOhtiw(Player *player, const std::vector<Polygon> &obstacles) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: obstacles) {
        // Check if a collision is detected
        if (checkSATCollision(player->getVertices(), obstacle)) {

            correctSATCollision(player, obstacle); // Correct the collision

            // If the collision is with the ground, the player is can't jump anymore
            if (checkSATCollision(player->getGroundColliderVertices(), obstacle)) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is can't jump anymore
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If collision detected with the roof, the player is on the platform
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                if (!platform.getAxis()) player->setX(player->getX() + platform.getMove());
                else player->setY(player->getY() + platform.getMove());
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is can't jump anymore
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the roof, the player is on a platform
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                player->setX(player->getX() + platform.getMoveX());
                player->setY(player->getY() + platform.getMoveY());
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
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the ground, the player is can't jump anymore
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setIsJumping(false);
                player->setMoveY(0);
            }
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



/* COLLISIONS UNRELATED TO MAVITY */

void handleCollisionsWithSizePowerUp(Player *player, Level *level, std::vector<SizePowerUp> &items) {
    // Check for collisions with each item
    for (SizePowerUp &item : items) {
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            level->removeItemFromSizePowerUp(item);
        }
    }
}

void handleCollisionsWithSpeedPowerUp(Player *player, Level *level, std::vector<SpeedPowerUp> &items) {
    // Check for collisions with each item
    for (SpeedPowerUp &item : items) {
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            level->removeItemFromSpeedPowerUp(item);
        }
    }
}