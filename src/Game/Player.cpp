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

SDL_Texture *Player::spriteTexture1MedalPtr = nullptr;
SDL_Texture *Player::spriteTexture2MedalPtr = nullptr;
SDL_Texture *Player::spriteTexture3MedalPtr = nullptr;
SDL_Texture *Player::spriteTexture4MedalPtr = nullptr;


/* CONSTRUCTORS */

Player::Player(int playerID, Point spawnPoint, float size)
        : playerID(playerID), x(spawnPoint.x), y(spawnPoint.y), size(size) {

    sprite = Sprite(*baseSpriteTexturePtr, Player::idle, BASE_SPRITE_WIDTH, BASE_SPRITE_HEIGHT);
    setSpriteTextureByID(playerID);
    setSize(size);
}


/* BASIC ACCESSORS */

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

float Player::getSize() const {
    return size;
}

int Player::getScore() const {
    return score;
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

bool Player::getIsGrounded() const {
    return isGrounded;
}

bool Player::getIsJumping() const {
    return isJumping;
}

size_t Player::getCurrentZoneID() const {
    return currentZoneID;
}

bool Player::getIsOnPlatform() const {
    return isOnPlatform;
}

bool Player::getWasOnPlatform() const {
    return wasOnPlatform;
}


/* SPECIFIC ACCESSORS */

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
    if (mavity > 0) return {
            {x, y + height},
            {x + width, y + height},
            {x + width, y + height + 1},
            {x, y + height + 1}
    };
    else return {
                {x, y - 1},
                {x + width, y - 1},
                {x + width, y},
                {x, y}
        };
}

std::vector<Point> Player::getRoofColliderVertices() const {
    if (mavity > 0) return {
            {x, y - 1},
            {x + width, y - 1},
            {x + width, y},
            {x, y}
    };
    else return {
                {x, y + height},
                {x + width, y + height},
                {x + width, y + height + 1},
                {x, y + height + 1}
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
    if (mavity > 0) return  {x, y + height, width, 1};
    else return {x, y - 1, width, 1};
}

SDL_FRect Player::getRoofColliderBoundingBox() const {
    if (mavity > 0) return {x, y - 1, width, 1};
    else return {x, y + height, width, 1};
}

SDL_FRect Player::getBoundingBox() const {
    return {x, y, width, height};
}

SDL_FRect Player::getBoundingBoxNextFrame() const {
    return {x + moveX, y + moveY, width, height};
}


/* MODIFIERS */

void Player::setX(float val) {
    x = val;
}

void Player::setY(float val) {
    y = val;
}

void Player::setSize(float val) {
    size = val;
    normalOffsets = {baseNormalOffsets.x * size, baseNormalOffsets.y * size, baseNormalOffsets.w * size, baseNormalOffsets.h * size};
    runOffsets = {baseRunOffsets.x * size, baseRunOffsets.y * size, baseRunOffsets.w * size, baseRunOffsets.h * size};
    spriteWidth = BASE_SPRITE_WIDTH * size;
    spriteHeight = BASE_SPRITE_HEIGHT * size;

    updateCollisionBox();
}

void Player::setMoveX(float val) {
    moveX = val;
}

void Player::setMoveY(float val) {
    moveY = val;
}

void Player::setBuffer(Buffer val) {
    buffer = val;
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

void Player::setIsGrounded(bool state) {
    isGrounded = state;
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

void Player::setCurrentZoneID(size_t id) {
    currentZoneID = id;
}

void Player::setMaxFallSpeed(float val) {
    maxFallSpeed = val;
}

void Player::setLeftCollider(bool state) {
    leftCollider = state;
}

void Player::setRightCollider(bool state) {
    rightCollider = state;
}

void Player::setRoofCollider(bool state) {
    roofCollider = state;
}

void Player::setGroundCollider(bool state) {
    groundCollider = state;
}

void Player::setIsOnPlatform(bool state) {
    isOnPlatform = state;
}

void Player::setWasOnPlatform(bool state) {
    wasOnPlatform = state;
}

void Player::setDefaultTexture(SDL_Texture* newTexture) {
    defaultTexture = newTexture;
}
void Player::setMedalTexture(SDL_Texture* newTexture) {
    medalTexture = newTexture;
}



/* METHODS */

bool Player::loadTextures(SDL_Renderer &renderer) {
    // Load players' sprite texture
    baseSpriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player.png");

    // Player 1
    spriteTexture1Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player1.png");
    spriteTexture1MedalPtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player1Medal.png");

    //Player 2
    spriteTexture2Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player2.png");
    spriteTexture2MedalPtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player2Medal.png");

    //Player 3
    spriteTexture3Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player3.png");
    spriteTexture3MedalPtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player3Medal.png");

    //Player 4
    spriteTexture4Ptr = IMG_LoadTexture(&renderer, "assets/sprites/players/player4.png");
    spriteTexture4MedalPtr = IMG_LoadTexture(&renderer, "assets/sprites/players/player4Medal.png");

    // Check errors
    if (baseSpriteTexturePtr == nullptr || spriteTexture1Ptr == nullptr || spriteTexture2Ptr == nullptr || spriteTexture3Ptr == nullptr || spriteTexture4Ptr == nullptr || spriteTexture1MedalPtr == nullptr || spriteTexture2MedalPtr == nullptr || spriteTexture3MedalPtr == nullptr || spriteTexture4MedalPtr == nullptr) {
        return false; // Return failure
    }

    return true; // Return success
}

void Player::useDefaultTexture() {
    sprite.setTexture(*defaultTexture);
}
void Player::useMedalTexture(){
    sprite.setTexture(*medalTexture);
};

void Player::setSpriteTextureByID(int id) {
    if (id == 1){ // Texture 1
        sprite.setTexture(*spriteTexture1Ptr);
        defaultTexture = spriteTexture1Ptr;
        medalTexture = spriteTexture1MedalPtr;
        baseNormalOffsets = {4, 1, 5, 0};
        baseRunOffsets = {6, 4, 0, 0};
    }
    else if (id == 2){ // Texture 2
        sprite.setTexture(*spriteTexture2Ptr);
        defaultTexture = spriteTexture2Ptr;
        medalTexture = spriteTexture2MedalPtr;
        baseNormalOffsets = {6, 1, 3, 0};
        baseRunOffsets = {6, 4, 0, 0};
    }
    else if (id == 3) { // Texture 3
        sprite.setTexture(*spriteTexture3Ptr);
        defaultTexture = spriteTexture3Ptr;
        medalTexture = spriteTexture3MedalPtr;
        baseNormalOffsets = {4, 1, 5, 0};
        baseRunOffsets = {6, 4, 0, 0};
    }
    else if (id == 4) { // Texture 4
        sprite.setTexture(*spriteTexture4Ptr);
        defaultTexture = spriteTexture4Ptr;
        medalTexture = spriteTexture4MedalPtr;
        baseNormalOffsets = {4, 1, 5, 0};
        baseRunOffsets = {6, 4, 0, 0};
    }
    else {
        sprite.setTexture(*baseSpriteTexturePtr);
        defaultTexture = baseSpriteTexturePtr;
        medalTexture = baseSpriteTexturePtr;
        baseNormalOffsets = {4, 1, 5, 0};
        baseRunOffsets = {6, 4, 0, 0};
    }
    setSize(size);
}

void Player::teleportPlayer(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::addToScore(int val) {
    score += val;
}

bool Player::canJump() const {
    return isGrounded || ((static_cast<float>(SDL_GetTicks()) - static_cast<float>(lastTimeOnPlatform)) / 1000.0f <= coyoteTime);
}

void Player::calculateXaxisMovement(double delta_time) {

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
            // If the direction changed, reset the speed curve
            speedCurveX = initialSpeedCurveX;
        } else {
            // Gradually increase the speed curve for smooth acceleration
            speedCurveX = static_cast<float>(std::min(speedCurveX + accelerationFactorX * delta_time, 1.0));
        }
    } else {
        // If no input, gradually decrease the speed curve for smooth deceleration
        speedCurveX = static_cast<float>(std::max(speedCurveX - decelerationFactorX * delta_time, 0.0));
    }

    // Calculate movement based on speed curve and direction
    moveX = static_cast<float>(baseMovementX * sprintMultiplier * delta_time * speedCurveX * directionX * speed);

    // Remember previous direction
    previousDirectionX = directionX;
}

void Player::calculateYaxisMovement(double delta_time) {

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
        if (std::abs(jumpStartHeight - y) > jumpMaxHeight || jumpVelocity <= 0 || !wantToJump) {
            isJumping = false;
            jumpStartHeight = 0;
            jumpVelocity = 0;
            wantToJump = false;
        }

        // The player is still jumping, calculate the jump movement for this frame
        else {
            jumpVelocity = jumpInitialVelocity - mavity * jumpHeight * 0.02f;

            // Calculate vertical movement based on jump velocity and time
            moveY = static_cast<float>((jumpVelocity * delta_time - 0.5f * mavity * delta_time * delta_time) * (mavity < 0 ? 1 : -1));

            // Update jump velocity for the next frame
            jumpVelocity -= mavity * static_cast<float>(delta_time);
        }
    }

    // If the player is not jumping, calculate the fall movement for this frame (if not on a platform)
    else if (!isGrounded)  {
        moveY += static_cast<float>(fallSpeedFactor * mavity * delta_time * delta_time);
        if (mavity > 0) {
            moveY = std::min(moveY, static_cast<float>(maxFallSpeed * delta_time));
        } else {
            moveY = std::max(moveY, static_cast<float>(-maxFallSpeed * delta_time));
        }
    }

    // Update the Y direction for collision correction
    if (moveY == 0) directionY = 0;
    else directionY = moveY < 0 ? -1 : 1;
}

void Player::calculateMovement(double delta_time) {
    calculateXaxisMovement(delta_time);
    calculateYaxisMovement(delta_time);
}

void Player::updateCollisionBox() {
    updateSpriteAnimation();
    updateSpriteOrientation();

    // Normal texture offsets
    if (sprite.getAnimation() == idle || sprite.getAnimation() == walk) {
        // Check horizontal orientation
        if (sprite.getFlipHorizontal() == SDL_FLIP_NONE) {
            textureOffsets.x = normalOffsets.x;
            textureOffsets.w = normalOffsets.w;
        }
        else {
            x -= textureOffsets.x - normalOffsets.w;
            textureOffsets.x = normalOffsets.w;
            textureOffsets.w = normalOffsets.x;
        }
        // Check vertical orientation
        if (mavity > 0) {
            y -= textureOffsets.y - normalOffsets.y;
            textureOffsets.y = normalOffsets.y;
            textureOffsets.h = normalOffsets.h;
        } else {
            textureOffsets.y = normalOffsets.h;
            textureOffsets.h = normalOffsets.y;
        }
    }
    // Run texture offsets
    else if (sprite.getAnimation() == run || sprite.getAnimation() == sneak) {
        // Check horizontal orientation
        if (sprite.getFlipHorizontal() == SDL_FLIP_NONE) {
            if (rightCollider) x -= textureOffsets.w - runOffsets.w;
            textureOffsets.x = runOffsets.x;
            textureOffsets.w = runOffsets.w;
        }
        else {
            if (!leftCollider) x -= textureOffsets.x - runOffsets.w;
            textureOffsets.x = runOffsets.w;
            textureOffsets.w = runOffsets.x;
        }
        // Check vertical orientation
        if (mavity > 0) {
            y -= textureOffsets.y - runOffsets.y;
            textureOffsets.y = runOffsets.y;
            textureOffsets.h = runOffsets.h;
        } else {
            textureOffsets.y = runOffsets.h;
            textureOffsets.h = runOffsets.y;
        }
    }

    // Update collision box
    width = spriteWidth - (textureOffsets.x + textureOffsets.w);
    height = spriteHeight - (textureOffsets.y + textureOffsets.h);
}

bool Player::hasMoved() const {
    return moveX != 0 || moveY != 0;
}

void Player::applyMovement(double delta_time) {

    // If the buffer is too big, move the player and decrease the buffer by the full amount
    if (buffer.deltaX > 40 || buffer.deltaX < -40 || buffer.deltaY > 40 || buffer.deltaY < -40) {
        std::cout << "Player: Buffer too big: " << buffer.deltaX << ", " << buffer.deltaY << std::endl;

        x = x + buffer.deltaX;
        y = y + buffer.deltaY;

        buffer.deltaX = 0;
        buffer.deltaY = 0;
    }

    else {
        // Add a part of the buffer to the player's position
        float bufferFraction = static_cast<float>(delta_time) * 1000.0f / 50.0f;

        x += moveX + bufferFraction * buffer.deltaX;
        y += moveY + bufferFraction * buffer.deltaY;

        // Decrease the buffer
        buffer.deltaX -= bufferFraction * buffer.deltaX;
        buffer.deltaY -= bufferFraction * buffer.deltaY;
    }
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
    sprite.updateAnimation();
    SDL_Rect srcRect = sprite.getSrcRect();

    float x_rect = x - camera.x - textureOffsets.x;
    float y_rect = y - camera.y - textureOffsets.y;
    float w_rect = width + textureOffsets.x + textureOffsets.w;
    float h_rect = height + textureOffsets.y + textureOffsets.h;

    SDL_FRect player_rect = {x_rect, y_rect, w_rect, h_rect};
    SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &player_rect, 0.0, nullptr, sprite.getFlip());
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