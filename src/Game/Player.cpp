#include "../../include/Game/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */

// Initialize textures pointers
SDL_Texture *Player::baseSpriteTexturePtr = nullptr;
SDL_Texture *Player::spriteTexture1Ptr = nullptr;
SDL_Texture *Player::spriteTexture2Ptr = nullptr;
SDL_Texture *Player::spriteTexture3Ptr = nullptr;
SDL_Texture *Player::spriteTexture4Ptr = nullptr;


/** CONSTRUCTORS **/

Player::Player(int playerID, Point spawnPoint, float playerWidth, float playerHeight)
        : playerID(playerID), x(spawnPoint.x), y(spawnPoint.y), width(playerWidth), height(playerHeight) {

    sprite = Sprite(Player::idle, *baseSpriteTexturePtr, 24, 18);
}


/** BASIC ACCESSORS **/

int Player::getPlayerID() const {
    return playerID;
}

short Player::getSpriteID() const {
    return SpriteID;
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

float Player::getW() const {
    return width;
}

float Player::getH() const {
    return height;
}

Sprite *Player::getSprite() {
    return &sprite;
}

float Player::getMoveX() const {
    return moveX;
}

float Player::getMoveY() const {
    return moveY;
}

int Player::getDirectionX() const {
    return (int)directionX;
}

bool Player::getCanMove() const {
    return canMove;
}

float Player::getSpeed() const {
    return speed;
}

bool Player::getWantToMoveRight() const {
    return wantToMoveRight;
}

bool Player::getWantToMoveLeft() const {
    return wantToMoveLeft;
}

float Player::getMavity() const {
    return mavity;
}

int Player::getDirectionY() const {
    return (int)directionY;
}

bool Player::getIsOnPlatform() const {
    return isOnPlatform;
}

bool Player::getIsJumping() const {
    return isJumping;
}


/** SPECIFIC ACCESSORS **/

std::vector<Point> Player::getVertices() const {
    // Return the vertices of the player's bounding box.
    return {
            {x, y},
            {x + width, y},
            {x + width, y + height},
            {x, y + height}
    };
}

std::vector<Point> Player::getVerticesNextFrame() const {
    return {
            {x + moveX, y + moveY},
            {x + moveX + width, y + moveY},
            {x + moveX + width, y + moveY + height},
            {x + moveX, y + moveY + height}
    };
}

std::vector<Point> Player::getHorizontalColliderVertices() const {
    return directionX == PLAYER_LEFT ? getLeftColliderVertices() : getRightColliderVertices();
}

std::vector<Point> Player::getLeftColliderVertices() const {
    return {
            {x - 1,y},
            {x, y},
            {x, y + height},
            {x - 1,y + height}
    };
}

std::vector<Point> Player::getRightColliderVertices() const {
    return {
            {x + width + 1, y},
            {x + width, y},
            {x + width, y + height},
            {x + width + 1, y + height}
    };
}

std::vector<Point> Player::getGroundColliderVertices() const {
    return {
            {x, y + height},
            {x + width, y + height},
            {x + width, y + height + 1},
            {x, y + height + 1}
    };
}

std::vector<Point> Player::getRoofColliderVertices() const {
    return {
            {x, y - 1},
            {x + width, y - 1},
            {x + width, y},
            {x, y}
    };
}

SDL_FRect Player::getHorizontalColliderBoundingBox() const {
    return directionX == PLAYER_LEFT ? getLeftColliderBoundingBox() : getRightColliderBoundingBox();
}

SDL_FRect Player::getLeftColliderBoundingBox() const {
    return {x - 1, y, 1, height};
}

SDL_FRect Player::getRightColliderBoundingBox() const {
    return {x + width, y, 1, height};
}

SDL_FRect Player::getGroundColliderBoundingBox() const {
    return {x, y + height, width, 1};
}

SDL_FRect Player::getRoofColliderBoundingBox() const {
    return {x, y - 1, width, 1};
}

SDL_FRect Player::getBoundingBox() const {
    return {x, y, width, height};
}

SDL_FRect Player::getBoundingBoxNextFrame() const {
    return {x + moveX, y + moveY, width, height};
}


/** MODIFIERS **/

void Player::setX(float val) {
    x = val;
}

void Player::setY(float val) {
    y = val;
}

void Player::setW(float val) {
    width = val;
}

void Player::setH(float val) {
    height = val;
}

void Player::setMoveX(float val) {
    moveX = val;
}

void Player::setMoveY(float val) {
    moveY = val;
}

void Player::setCanMove(bool state) {
    canMove = state;
}

void Player::setSpeed(float val) {
    speed = val;
}

void Player::setWantToMoveRight(bool state) {
    wantToMoveRight = state;
}

void Player::setWantToMoveLeft(bool state) {
    wantToMoveLeft = state;
}

void Player::setSprint(bool state) {
    if (state) {
        sprintMultiplier = 1.3f;
    } else {
        sprintMultiplier = 1.0;
    }
}

void Player::setIsOnPlatform(bool state) {
    isOnPlatform = state;
    if (state) {
        lastTimeOnPlatform = SDL_GetTicks();
    }
}

void Player::setWantToJump(bool state) {
    wantToJump = state;

    if (!state) {
        jumpLock = false;
    }
}

void Player::setIsJumping(bool state) {
    isJumping = state;
}

void Player::toggleMavity() {
    mavity *= -1;
}

void Player::setPlayerID(int id) {
    playerID = id;
}

void Player::setSpriteID(short id) {
    SpriteID = id;
}


/** METHODS **/

bool Player::loadTextures(SDL_Renderer &renderer) {
    // Load players' sprite texture
    baseSpriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player.png");
    spriteTexture1Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player1.png");
    spriteTexture2Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player2.png");
    spriteTexture3Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player3.png");
    spriteTexture4Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player4.png");

    // Check errors
    if (baseSpriteTexturePtr == nullptr || spriteTexture1Ptr == nullptr || spriteTexture2Ptr == nullptr || spriteTexture3Ptr == nullptr || spriteTexture4Ptr == nullptr) {
        return false; // Return failure
    }

    return true; // Return success
}

void Player::setSpriteTextureByID(int id) {
    if (id == 0) sprite.setTexture(*baseSpriteTexturePtr); // Base texture
    else if (id == 1) sprite.setTexture(*spriteTexture1Ptr); // Texture 1
    else if (id == 2) sprite.setTexture(*spriteTexture2Ptr); // Texture 2
    else if (id == 3) sprite.setTexture(*spriteTexture3Ptr); // Texture 3
    else if (id == 4) sprite.setTexture(*spriteTexture4Ptr); // Texture 4
}

void Player::teleportPlayer(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::addToScore(int val) {
    score += val;
}

bool Player::canJump() const {
    return isOnPlatform || ((static_cast<float>(SDL_GetTicks()) - static_cast<float>(lastTimeOnPlatform)) / 1000.0f <= coyoteTime);
}

void Player::calculateXaxisMovement(double deltaTime) {

    // Determine the desired direction
    float wantedDirection = 0;
    if (wantToMoveLeft && !wantToMoveRight) {
        wantedDirection = -1;
    } else if (!wantToMoveLeft && wantToMoveRight) {
        wantedDirection = 1;
    }

    // Update direction and speed curve
    if (wantedDirection != 0) {
        directionX = wantedDirection;
        if (directionX != previousDirectionX && speedCurveX != 0) {
            // If direction changed, reset speed curve
            speedCurveX = initialSpeedCurveX;
        } else {
            // Gradually increase speed curve for smooth acceleration
            speedCurveX = static_cast<float>(std::min(speedCurveX + accelerationFactorX * deltaTime, 1.0));
        }
    } else {
        // If no input, gradually decrease speed curve for smooth deceleration
        speedCurveX = static_cast<float>(std::max(speedCurveX - decelerationFactorX * deltaTime, 0.0));
    }

    // Calculate movement based on speed curve and direction
    moveX = static_cast<float>(baseMovementX * sprintMultiplier * deltaTime * speedCurveX * directionX * speed);

    // Remember previous direction
    previousDirectionX = directionX;
}

void Player::calculateYaxisMovement(double deltaTime) {

    // If the player wants to jump and can jump, start the jump
    if (!jumpLock && wantToJump && canJump() && !isJumping) {
        isJumping = true;
        jumpLock = true;
        jumpStartHeight = y; // Record the height at the beginning of the jump
        jumpVelocity = jumpInitialVelocity; // Set the initial jump velocity
    }

    // If the player is jumping, calculate the jump movement for this frame
    else if (isJumping) {
        // Calculate the jump height
        float jumpHeight = jumpStartHeight - y;

        // Check if the player has finished the jump (with a very very very small margin of error)
        if (jumpStartHeight - y > jumpMaxHeight || jumpVelocity <= 0 || !wantToJump) {
            isJumping = false;
            jumpStartHeight = 0;
            jumpVelocity = 0;
            wantToJump = false;
        }

        // The player is still jumping, calculate the jump movement for this frame
        else {
            jumpVelocity = jumpInitialVelocity - mavity * jumpHeight * 0.02f;

            // Calculate vertical movement based on jump velocity and time
            moveY = static_cast<float>((jumpVelocity * deltaTime - 0.5f * mavity * deltaTime * deltaTime) * (mavity < 0 ? 1 : -1));

            // Update jump velocity for the next frame
            jumpVelocity -= mavity * static_cast<float>(deltaTime);
        }
    }

    // If the player is not jumping, calculate the fall movement for this frame (if not on a platform)
    else if (!isOnPlatform)  {
        moveY += static_cast<float>(fallSpeedFactor * mavity * deltaTime * deltaTime);
        if (mavity > 0) {
            moveY = std::min(moveY, static_cast<float>(maxFallSpeed * deltaTime));
        } else {
            moveY = std::max(moveY, static_cast<float>(-maxFallSpeed * deltaTime));
        }
    }

    // Update the Y direction for collision correction
    if (moveY == 0) directionY = 0;
    else directionY = moveY < 0 ? -1 : 1;
}

void Player::calculateMovement(double deltaTime) {
    calculateXaxisMovement(deltaTime);
    calculateYaxisMovement(deltaTime);
}

bool Player::hasMoved() const {
    return moveX != 0 || moveY != 0;
}

void Player::applyMovement() {
    x += moveX;
    y += moveY;
}

void Player::updateSpriteAnimation() {
    // If the player doesn't move, play idle or sneak animation
    if (wantToMoveLeft == 0 && wantToMoveRight == 0) {
        if (sprintMultiplier == 1) sprite.setAnimation(idle);
        else sprite.setAnimation(sneak);
    }
    // If the player is moving, play walk or run animation
    else {
        if (sprintMultiplier == 1) {
            sprite.setAnimation(walk);
        } else {
            // If the current animation is 'sneak', start the run animation at the second frame
            if (sprite.getAnimation() == sneak) {
                sprite.setAnimation(run);
                sprite.setAnimationIndexX(1);
            } else {
                sprite.setAnimation(run);
            }
        }
    }
}

void Player::updateSpriteOrientation() {
     if (directionX == PLAYER_LEFT) {
        sprite.setFlipHorizontal(SDL_FLIP_HORIZONTAL);
    } else {
        sprite.setFlipHorizontal(SDL_FLIP_NONE);
    }
}


void Player::render(SDL_Renderer *renderer, Point camera) {
    // Update sprite
    updateSpriteAnimation();
    updateSpriteOrientation();
    sprite.updateAnimation();

    SDL_Rect srcRect = sprite.getSrcRect();
    SDL_FRect playerRect = {x - camera.x, y - camera.y, width, height};
    SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &playerRect, 0.0, nullptr, sprite.getFlip());
}

void Player::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect playerRect = {x - camera.x, y - camera.y, width, height};
    SDL_RenderFillRectF(renderer, &playerRect);
}

void Player::renderColliders(SDL_Renderer *renderer, Point camera) const {
    //Draw the right collider
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    std::vector<Point> vertexRight = getRightColliderVertices();
    for (size_t i = 0; i < vertexRight.size(); ++i) {
        const auto &vertex1 = vertexRight[i];
        const auto &vertex2 = vertexRight[(i + 1) % vertexRight.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the left collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    std::vector<Point> vertexLeft = getLeftColliderVertices();
    for (size_t i = 0; i < vertexLeft.size(); ++i) {
        const auto &vertex1 = vertexLeft[i];
        const auto &vertex2 = vertexLeft[(i + 1) % vertexLeft.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the roof collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertex = getRoofColliderVertices();
    for (size_t i = 0; i < vertex.size(); ++i) {
        const auto &vertex1 = vertex[i];
        const auto &vertex2 = vertex[(i + 1) % vertex.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the ground collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertexGround = getGroundColliderVertices();
    for (size_t i = 0; i < vertexGround.size(); ++i) {
        const auto &vertex1 = vertexGround[i];
        const auto &vertex2 = vertexGround[(i + 1) % vertexGround.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
}