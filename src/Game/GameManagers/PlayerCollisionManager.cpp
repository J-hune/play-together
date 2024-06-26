#include "../../../include/Game/GameManagers/PlayerCollisionManager.h"
#include "../../../dependencies/json.hpp"

/**
 * @file PlayerCollisionManager.cpp
 * @brief Implements the PlayerCollisionManager class responsible for handling player collision behavior.
 */


/* CONSTRUCTORS */

PlayerCollisionManager::PlayerCollisionManager(Game *game) : gamePtr(game) {}


/* METHODS */

void PlayerCollisionManager::handleCollisionsWithObstacles(Player *player) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: gamePtr->getBroadPhaseManager().getObstacles()) {
        // Check if a collision is detected
        if (checkSATCollision(player->getVertices(), obstacle)) {

            correctSATCollision(player, obstacle); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkSATCollision(player->getRoofColliderVertices(), obstacle)) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on the ground
            if (!player->getIsGrounded() && checkSATCollision(player->getGroundColliderVertices(), obstacle)) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
            }
        }
        // If the collision is with the wall, the player can't move
        if (player->getCanMove() && checkSATCollision(player->getHorizontalColliderVertices(), obstacle)) {
            player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
            player->setCanMove(false);
        }
    }
}

bool PlayerCollisionManager::handleCollisionsWithTreadmillLevers(const Player *player) {
    // Check for collisions with each lever
    for (const TreadmillLever &lever: gamePtr->getBroadPhaseManager().getTreadmillLevers()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getHitZoneBoundingBox(), lever.getBoundingBox())) {
            // If the player is on the lever, activate it
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), lever.getBoundingBox())) {
                gamePtr->getLevel()->activateTreadmillLever(lever);
                return true;
            }
        }
    }
    return false;
}

bool PlayerCollisionManager::handleCollisionsWithPlatformLevers(const Player *player) {
    // Check for collisions with each lever
    for (const PlatformLever &lever: gamePtr->getBroadPhaseManager().getPlatformLevers()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getHitZoneBoundingBox(), lever.getBoundingBox())) {
            // If the player is on the lever, activate it
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), lever.getBoundingBox())) {
                gamePtr->getLevel()->activatePlatformLever(lever);
                return true;
            }
        }
    }
    return false;
}

bool PlayerCollisionManager::handleCollisionsWithCrusherLevers(const Player *player) {
    // Check for collisions with each lever
    for (const CrusherLever &lever: gamePtr->getBroadPhaseManager().getCrusherLevers()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getHitZoneBoundingBox(), lever.getBoundingBox())) {
            // If the player is on the lever, activate it
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), lever.getBoundingBox())) {
                gamePtr->getLevel()->activateCrusherLever(lever);
                return true;
            }
        }
    }
    return false;
}

void PlayerCollisionManager::handleCollisionsWithMovingPlatform1D(Player *player) {
    // Check for collisions with each 1D moving platform
    for (const MovingPlatform1D &platform: gamePtr->getBroadPhaseManager().getMovingPlatforms1D()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                if (!platform.getAxis()) player->setX(player->getX() + platform.getMove());
                else player->setY(player->getY() + platform.getMove());
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                player->setCanMove(false);
            }
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithMovingPlatform2D(Player *player) {

    // Check for collisions with each 2D moving platform
    for (const MovingPlatform2D &platform: gamePtr->getBroadPhaseManager().getMovingPlatforms2D()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
                player->setIsOnPlatform(true);
                // Add platform velocity to the player
                player->setX(player->getX() + platform.getMoveX());
                player->setY(player->getY() + platform.getMoveY());
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                player->setCanMove(false);
            }
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithSwitchingPlatform(Player *player) {

    // Check for collisions with each switching platform
    for (const SwitchingPlatform &platform: gamePtr->getBroadPhaseManager().getSwitchingPlatforms()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
                player->setIsOnPlatform(true);
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                player->setCanMove(false);
            }
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithWeightPlatform(Player *player) {
    // Check for collisions with each switching platform
    for (const WeightPlatform &platform: gamePtr->getBroadPhaseManager().getWeightPlatforms()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getBoundingBox(), platform.getBoundingBox())) {

            correctAABBCollision(player, platform.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
                player->setIsOnPlatform(true);
                if (player->getMavity() > 0) gamePtr->getLevel()->increaseWeightForPlatform(platform);
                else gamePtr->getLevel()->decreaseWeightForPlatform(platform);
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
                player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                player->setCanMove(false);
            }
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithTreadmills(Player *player) {
    // Check for collisions with each treadmill
    for (const Treadmill &treadmill: gamePtr->getBroadPhaseManager().getTreadmills()) {
        // Check if a collision is detected
        if (checkAABBCollision(player->getBoundingBox(), treadmill.getBoundingBox())) {

            correctAABBCollision(player, treadmill.getBoundingBox()); // Correct the collision

            // If the collision is with the roof, the player can't jump anymore
            if (checkAABBCollision(player->getRoofColliderBoundingBox(), treadmill.getBoundingBox())) {
                player->setRoofCollider(true);
                player->setIsJumping(false);
                player->setMoveY(0);
            }
            // If the collision is with the ground, the player is on a platform
            if (checkAABBCollision(player->getGroundColliderBoundingBox(), treadmill.getBoundingBox())) {
                player->setGroundCollider(true);
                player->setIsGrounded(true);
                player->setIsOnPlatform(true);

                // Add treadmill velocity to the player
                if (treadmill.getIsMoving()) {
                    float move = treadmill.getMove();
                    if (player->getMavity() < 0) move *= -1; // Apply mavity
                    player->setX(player->getX() + move);
                }
            }
            // If the collision is with the wall, the player can't move
            if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), treadmill.getBoundingBox())) {
                player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                player->setCanMove(false);
            }
        }
    }

}

bool PlayerCollisionManager::handleCollisionsWithCrushers(Player *player) {
    // Check for collisions with each crusher
    for (Crusher const &crusher : gamePtr->getBroadPhaseManager().getCrushers()) {
        // Check if a collision is detected
            if (checkAABBCollision(player->getBoundingBox(), crusher.getBoundingBox())) {
                // If the player is being crushed, return true to kill him
                if (crusher.getIsCrushing() && checkAABBCollision(player->getBoundingBox(), crusher.getCrushingZoneBoundingBox())) {
                    return true;
                }
                // If the player is not being crushed, correct the collision
                else {

                    correctAABBCollision(player, crusher.getBoundingBox());

                    // If the collision is with the roof, the player can't jump anymore
                    if (checkAABBCollision(player->getRoofColliderBoundingBox(), crusher.getBoundingBox())) {
                        player->setRoofCollider(true);
                        player->setIsJumping(false);
                        player->setMoveY(0);
                    }
                    // If the collision is with the ground, the player is on a platform
                    if (checkAABBCollision(player->getGroundColliderBoundingBox(), crusher.getBoundingBox())) {
                        player->setGroundCollider(true);
                        player->setIsGrounded(true);
                        player->setIsOnPlatform(true);
                        player->setMoveY(0);
                    }
                    // If the collision is with the wall, the player can't move
                    if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), crusher.getBoundingBox())) {
                        player->getDirectionX() > 0 ? player->setRightCollider(true) : player->setLeftCollider(true);
                        player->setCanMove(false);
                    }
                }
            }
        }

    return false;
}

bool PlayerCollisionManager::handleCollisionsWithCameraBorders(const SDL_FRect player) {
    const SDL_FRect &camera = gamePtr->getCamera()->getBoundingBox();

    return player.x < camera.x - DISTANCE_OUT_MAP_BEFORE_DEATH                           // Left border
           || player.x + player.w > camera.x + camera.w + DISTANCE_OUT_MAP_BEFORE_DEATH  // Right border
           || player.y < camera.y - DISTANCE_OUT_MAP_BEFORE_DEATH                        // Top border
           || player.y + player.h > camera.y + camera.h + DISTANCE_OUT_MAP_BEFORE_DEATH; // Bottom border
}

bool PlayerCollisionManager::handleCollisionsWithDeathZones(const Player &player) {
    size_t i = 0;
    const std::vector<Polygon> &deathZones = gamePtr->getBroadPhaseManager().getDeathZones();

    // Check collisions with each death zone until the player is dead
    while (i < deathZones.size() && !checkSATCollision(player.getVertices(), deathZones[i])) {
        i++;
    }

    return i != deathZones.size(); // True if the player collided with a death zone
}

void PlayerCollisionManager::handleCollisionsWithSaveZones(Player &player) {
    // Check collisions with each save zone
    for (const AABB &save_zone : gamePtr->getBroadPhaseManager().getSaveZones()) {
        // Check a collision is detected and the zone has not yet been reached
        if (static_cast<int>(player.getCurrentZoneID()) == (save_zone.getID() + 1)) return;
        if (checkAABBCollision(player.getBoundingBox(), save_zone.getRect()) && gamePtr->getLevel()->getLastCheckpoint() < save_zone.getID()) {
            gamePtr->getLevel()->setLastCheckpoint(static_cast<short>(save_zone.getID()));
            player.setCurrentZoneID(save_zone.getID() + 1);
            std::cout << "Checkpoint reached: " << save_zone.getID() << std::endl;
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithRescueZones(const Player &player) {
    // Check collisions with each rescue zone
    for (const AABB &zone : gamePtr->getBroadPhaseManager().getRescueZones()) {
        // Check a collision is detected
        if (checkAABBCollision(player.getBoundingBox(), zone.getRect())) {
            gamePtr->getPlayerManager().setCurrentRescueZone(zone);
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithToggleGravityZones(Player &player, double delta_time) {
    const std::vector<AABB> &toggleGravityZones = gamePtr->getBroadPhaseManager().getToggleGravityZones();

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

            float gravityOffset = toggleGravityZone.getHeight() * static_cast<float>(delta_time) * 30;
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

void PlayerCollisionManager::handleCollisionsWithIncreaseFallSpeedZones(Player &player) {
    // Check collisions with each increase fall speed zone
    for (const auto & increaseFallSpeedZone : gamePtr->getBroadPhaseManager().getIncreaseFallSpeedZones()) {
        // Check if the player is already in the increase fall speed zone
        if (checkAABBCollision(player.getBoundingBox(), increaseFallSpeedZone.getRect())) {
            player.setMaxFallSpeed(1300);
        } else {
            player.setCurrentZoneID(0);
            player.setMaxFallSpeed(600);
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithSizePowerUps(Player *player) {
    // Check for collisions with each item
    for (SizePowerUp &item : gamePtr->getBroadPhaseManager().getSizePowerUps()) {
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            gamePtr->getLevel()->removeItemFromSizePowerUp(item);
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithSpeedPowerUps(Player *player) {
    // Check for collisions with each item
    for (SpeedPowerUp &item : gamePtr->getBroadPhaseManager().getSpeedPowerUps()) {
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            gamePtr->getLevel()->removeItemFromSpeedPowerUp(item);
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithCoins(Player *player) {
    // Check for collisions with each item
    for (Coin &item : gamePtr->getBroadPhaseManager().getCoins()) {
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item.getBoundingBox())) {
            item.applyEffect(*player);
            gamePtr->getLevel()->removeItemFromCoins(item);
        }
    }
}

void PlayerCollisionManager::handleCollisionsWithItem(Player *player) {
    //section check if the time of the power has not passed

    //checks if the queue is empty
    if(!timeQueue.empty()){
        int count = 0;
        //keep the current size of the queue
        int  currentSizeQueue = timeQueue.size();
        while( count < currentSizeQueue ){
            //we get the first element of the queue
            GameData* current = timeQueue.front();
            //check if the time of the power has passed
            if (difftime(time(nullptr) ,current->t) >= 4){
                //we remove the element form the queue since is not necessary
                current->item->inverseEffect(*player);
                timeQueue.pop();
                //free the allocated memory
                free(current);
                //check for the rest elements of the queue
                count++;
            }else{
                //in means that all the other elements have smaller time difference
                // -> not necessary to have a further check
                count = currentSizeQueue;
            }
        }
    }

    //section to get the new power
    for(Item* item : gamePtr->getLevel()->getItems()){
        // If a collision is detected, apply item's effect to the player and erase it
        if (checkAABBCollision(player->getBoundingBox(), item->getBoundingBox())) {
            item->applyEffect(*player);
            auto* data = static_cast<GameData *>(calloc(1, sizeof(GameData)));
            //initialise the gameData
            time(&data->t);
            data->item = item;
            timeQueue.push(data);
            gamePtr->getLevel()->removeItem(*item);
        }
    }

}

bool PlayerCollisionManager::handleCollisionsWithDeadPlayers(const Player *player) {
    // Check for collisions with each dead player
    for (Player &dead_player : gamePtr->getPlayerManager().getDeadPlayers()) {
        // If a collision is detected, respawn the dead player
        if (checkAABBCollision(player->getHitZoneBoundingBox(), dead_player.getBoundingBox())) {
            gamePtr->getPlayerManager().respawnPlayer(dead_player);
            return true;
        }
    }
    return false;
}

void PlayerCollisionManager::handleCollisions(double delta_time) {
    // Handle collisions for each living player
    for (Player &player: gamePtr->getPlayerManager().getAlivePlayers()) {
        player.updateCollisionBox();

        // Handle collisions with death zones and camera borders to check if the player dies
        if (handleCollisionsWithCameraBorders(player.getBoundingBox())
            || handleCollisionsWithDeathZones(player)
            || handleCollisionsWithCrushers(&player))
        {
            gamePtr->getPlayerManager().killPlayer(player);
        }
        // Check every other collision if the player is alive
        else {
            player.setLeftCollider(false);
            player.setRightCollider(false);
            player.setRoofCollider(false);
            player.setGroundCollider(false);
            player.setCanMove(true);
            player.setIsGrounded(false);
            player.setIsOnPlatform(false);

            if (player.hasMoved()) handleCollisionsWithObstacles(&player); // Handle collisions with obstacles

            // Handle collisions with platforms
            handleCollisionsWithMovingPlatform1D(&player);
            handleCollisionsWithMovingPlatform2D(&player);
            handleCollisionsWithSwitchingPlatform(&player);
            handleCollisionsWithWeightPlatform(&player);
            handleCollisionsWithTreadmills(&player);

            // Check if the player leaves a platform (this is what we call in French "bidouillage")
            if (player.getWasOnPlatform() && !player.getIsOnPlatform()) {
                player.setWasOnPlatform(false);
                player.setMoveY(0);
            }
            player.setWasOnPlatform(player.getIsOnPlatform());

            // Handle collisions with hit zone
            if (player.getIsHitting() && (handleCollisionsWithTreadmillLevers(&player)
                                        || handleCollisionsWithPlatformLevers(&player)
                                        || handleCollisionsWithCrusherLevers(&player)
                                        || handleCollisionsWithDeadPlayers(&player))) {

                player.setIsHitting(false);
            }

            // Handle collisions with items
            handleCollisionsWithItem(&player);
            handleCollisionsWithCoins(&player);

            handleCollisionsWithSaveZones(player); // Handle collisions with save zones
            handleCollisionsWithRescueZones(player); // Handle collisions with rescue zones
            handleCollisionsWithToggleGravityZones(player, delta_time); // Handle collisions with toggle gravity zones
            handleCollisionsWithIncreaseFallSpeedZones(player); // Handle collisions with increase fall speed zones
        }
    }

    // Handle collisions for each dead player
    for (Player &player: gamePtr->getPlayerManager().getDeadPlayers()) {
        player.setRoofCollider(false);
        player.setGroundCollider(false);
        player.setIsGrounded(false);

        handleCollisionsWithObstacles(&player); // Handle collisions with obstacles
    }
}