#include "../../../include/Game/Objects/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */

// Initialize texture pointers
SDL_Texture *Player::baseSpriteTexturePtr = nullptr;
SDL_Texture *Player::spriteTexture1Ptr = nullptr;
SDL_Texture *Player::spriteTexture2Ptr = nullptr;
SDL_Texture *Player::spriteTexture3Ptr = nullptr;
SDL_Texture *Player::spriteTexture4Ptr = nullptr;


/** CONSTRUCTORS **/

Player::Player(float startX, float startY, float playerSpeed, float playerWidth, float playerHeight)
        : x(startX), y(startY), speed(playerSpeed), width(playerWidth), height(playerHeight) {

    sprite = Sprite(Player::idle, *baseSpriteTexturePtr, 24, 18);
}


/** BASIC ACCESSORS **/

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

float Player::getSpeed() const {
    return speed;
}

Sprite* Player::getSprite() {
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

bool Player::getWantToMoveRight() const {
    return wantToMoveRight;
}

bool Player::getWantToMoveLeft() const {
    return wantToMoveLeft;
}

float Player::getMavity() const {
    return mavity;
}

bool Player::getWantToJump() const {
    return wantToJump;
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
    // Return the vertices of the player's bounding box.
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
    return {x, y - 1 , width, 1};
}

SDL_FRect Player::getBoundingBox() const {
    // Return the bounding box of the player.
    return {x, y, width, height};
}

SDL_FRect Player::getBoundingBoxNextFrame() const {
    // Return the bounding box of the player.
    return {x + moveX, y + moveY, width, height};
}


/** MODIFIERS **/

void Player::setX(float val){
    x = val;
}

void Player::setY(float val){
    y = val;
}

void Player::setW(float val){
    width = val;
}

void Player::setH(float val){
    height = val;
}

void Player::setMoveX(float val){
    moveX = val;
}

void Player::setMoveY(float val){
    moveY = val;
}

void Player::setCanMove(bool state) {
    canMove = state;
}

void Player::setWantToMoveRight(bool state) {
    wantToMoveRight = state;
}

void Player::setWantToMoveLeft(bool state) {
    wantToMoveLeft = state;
}

void Player::setIsOnPlatform(bool state) {
    isOnPlatform = state;
}

void Player::setWantToJump(bool state) {
    wantToJump = state;
}

void Player::setTimeSpentJumping(float val) {
    timeSpentJumping = val;
}

void Player::toggleMavity() {
    mavity *= -1;
}


/** METHODS **/

bool Player::loadTextures(SDL_Renderer &renderer) {
    // Load players' sprite texture
    baseSpriteTexturePtr = IMG_LoadTexture(&renderer, "../assets/sprites/players/player.png");
    spriteTexture1Ptr = IMG_LoadTexture(&renderer, "../assets/sprites/players/player1.png");
    spriteTexture2Ptr = IMG_LoadTexture(&renderer, "../assets/sprites/players/player2.png");
    spriteTexture3Ptr = IMG_LoadTexture(&renderer, "../assets/sprites/players/player3.png");
    spriteTexture4Ptr = IMG_LoadTexture(&renderer, "../assets/sprites/players/player4.png");

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

bool Player::canJump() {
    return isOnPlatform || ((float)SDL_GetTicks() - (float)lastTimeOnPlatform < coyoteTime * 1000);
}

void Player::calculateXaxisMovement(float deltaTime) {
    // Determine the direction
    float wantedDirection = 0;
    if (wantToMoveLeft && !wantToMoveRight) {
        wantedDirection = -1;
    } else if (!wantToMoveLeft && wantToMoveRight) {
        wantedDirection = 1;
    }

    // The player want to move
    if (wantedDirection != 0) {
        directionX = wantedDirection;

        if (speedCurve >= 1) {
            speedCurve = 1;
        } else {
            speedCurve += (((float)SDL_GetTicks() - (float)moveStartTime) / 1000) * speedCurveLerp;
            if (speedCurve > 1) speedCurve = 1;
        }

        moveStopTime = SDL_GetTicks();
    }

    // The player doesn't want to move
    else {
        // The player doesn't move
        if (speedCurve <= speedCurveMin) {
            speedCurve = speedCurveMin;
            directionX = 0;
        }
        // The player is in his deceleration curve
        else {
            speedCurve -= (((float)SDL_GetTicks() - (float)moveStopTime) / 1000) * speedCurveLerp;
            if (speedCurve < speedCurveMin) speedCurve = speedCurveMin;
        }

        moveStartTime = SDL_GetTicks();
    }

    moveX = 350; // Add basic movement
    moveX *= deltaTime; // Apply movement per second
    moveX *= speed; // Apply speed
    moveX *= speedCurve; // Apply the speed curve
    moveX *= directionX; // Apply direction
}

void Player::calculateYaxisMovement(float deltaTime) {

    // The player is in the first jump phase
    if (wantToJump && timeSpentJumping < PRESSURE_JUMP_MAX) {
        isJumping = true;
        moveY = -(400 * timeSpentJumping - 0.3F * (timeSpentJumping * timeSpentJumping)) * deltaTime * mavity;
        timeSpentJumping = ((float)SDL_GetTicks() - (float)lastTimeOnPlatform) / 1000;
    }
    // The player is in the second jump phase
    else if (timeSpentJumping > 0 && timeSpentJumping < PRESSURE_JUMP_MIN) {
        moveY = -(400 * timeSpentJumping - 0.3F * (timeSpentJumping * timeSpentJumping)) * deltaTime * mavity;
        timeSpentJumping = ((float)SDL_GetTicks() - (float)lastTimeOnPlatform) / 1000;
    }
    // The player is not jumping
    else {
        timeSpentJumping = 0;
        wantToJump = false;
        // The player is on a platform
        if (isOnPlatform) {
            moveY = 0;
            isJumping = false;
            lastTimeOnPlatform = SDL_GetTicks();
        }
        // The player is falling
        else {
            moveY = 400; // Add basic movement
            moveY *= deltaTime; // Apply movement per second
            moveY *= mavity; // Apply gravity
        }
    }

    // Calculate y-axis direction (used for collision correction)
    if (moveY == 0) directionY = 0;
    else directionY = moveY < 0 ? -1 : 1;
}

void Player::calculateMovement(float deltaTime) {
    calculateXaxisMovement(deltaTime);
    calculateYaxisMovement(deltaTime);
}

void Player::render(SDL_Renderer *renderer, Point camera) {
    sprite.updateAnimation(); // Update sprite animation
    SDL_Rect srcRect = sprite.getSrcRect();
    SDL_FRect playerRect = {x - camera.x, y - camera.y, width, height};
    SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &playerRect, 0.0, nullptr, sprite.getFlip());
}

void Player::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //printf("PLAYERY = %f\n", y - camera.y);
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