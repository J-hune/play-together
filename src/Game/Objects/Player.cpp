#include "../../../include/Game/Objects/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */


/** CONSTRUCTOR **/

Player::Player(float startX, float startY, float playerSpeed,float speedMax, float playerWidth, float playerHeight, Sprite &sprite)
        : x(startX), y(startY), speed(playerSpeed), speedMax(speedMax), width(playerWidth), height(playerHeight), sprite(sprite) {

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

float Player::getTimeAfterFall() const {
    return timeAfterFall;
}

int Player::getCurrentDirection() const {
    return currentDirection;
}

int Player::getDesiredDirection() const {
    return desiredDirection;
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

bool Player::getIsOnPlatform() const {
    return isOnPlatform;
}

bool Player::getIsJumping() const {
    return isJumping;
}

bool Player::getWantToJump() const {
    return wantToJump;
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

std::vector<Point> Player::getVerticesHorizontal() const {
    return currentDirection == PLAYER_LEFT ? getVerticesLeft() : getVerticesRight();
}

std::vector<Point> Player::getVerticesLeft() const {
    // Return the vertices of the player's bounding box, with added margin to capture left wall within the area.
    return {
            {x + (- 2),         y + 2},
            {x + ( 2), y + 2},
            {x + ( 2), y + height - 2},
            {x + (- 2),         y + height - 2}
    };
}

std::vector<Point> Player::getVerticesRight() const {
    // Return the vertices of the player's bounding box, with added margin to capture right wall within the area.
    return {
            {width + x + (2),         y + 2},
            {width + x + (-2), y + 2},
            {width + x + (-2), y + height - 2},
            {width + x + (2),         y + height - 2}
    };
}
std::vector<Point> Player::getVerticesGround() const {
    // Return the vertices of the player's bounding box, with added margin to capture ground within the area.
    return {
            {x + 2,         y + height},
            {x + width - 2, y + height},
            {x + width - 2, y + height + 2},
            {x + 2,         y + height + 2}
    };
}
std::vector<Point> Player::getVerticesRoof() const {
    // Return the vertices of the player's bounding box, with added margin to capture roof within the area.
    return {
            {x + 2,         y + 2},
            {x + width - 2, y + 2},
            {x + width - 2,  y },
            {x + 2,         y }
    };
}

SDL_FRect Player::getHorizontalColliderBoundingBox() const {
    return currentDirection == PLAYER_LEFT ? getLeftColliderBoundingBox() : getRightColliderBoundingBox();
}

SDL_FRect Player::getLeftColliderBoundingBox() const {
    // Return the vertices of the player's bounding box, with added margin to capture roof within the area.
    return {x - 2, y, 2, height};
}

SDL_FRect Player::getRightColliderBoundingBox() const {
    // Return the vertices of the player's bounding box, with added margin to capture roof within the area.
    return {x + width, y, 2, height};
}

SDL_FRect Player::getGroundColliderBoundingBox() const {
    // Return the vertices of the player's bounding box, with added margin to capture roof within the area.
    return {x, y + height, width, 2};
}

SDL_FRect Player::getRoofColliderBoundingBox() const {
    return {x, y - 2 , width, 2};
}

SDL_FRect Player::getBoundingBox() const {
    // Return the bounding box of the player.
    return {x, y, width, height};
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


void Player::setFinishTheMovement(bool state){
    finishTheMovement = state;
}

void Player::setTimeSpeed(float val){
    timeSpeed = val;
}

void Player::setDesiredDirection(int val) {
    desiredDirection = val;
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

void Player::setTimeSpentJumping(float val) {
    timeSpentJumping = val;
}

void Player::setIsOnPlatform(bool state) {
    isOnPlatform = state;
}

void Player::setWantToJump(bool state) {
    wantToJump = state;
}


/** METHODS **/

void Player::teleportPlayer(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::calculateMovement() {

    // The player move on the x-axis
    if (finishTheMovement && (wantToMoveLeft || wantToMoveRight)) {
        timeSpeed += 0.1F;
        moveX = speedMax * speed * timeSpeed;
        if (moveX > speedMax) {
            moveX = speedMax;
            timeSpeed = maxSpeedReachWithThisTime;
        }
        moveX *= (float)desiredDirection;
        currentDirection = desiredDirection;
    }
        // The player doesn't move on the x-axis
    else {
        if (timeSpeed > 0) {
            timeSpeed -= 0.05F;
            moveX = speedMax * speed * timeSpeed;
            moveX *= (float)currentDirection;
            finishTheMovement = false;
        } else {
            timeSpeed = 0;
            moveX = 0;
            finishTheMovement = true;
        }
    }

    // The player press "jump button" and he doesn't maintain more than 6
    if (wantToJump && timeSpentJumping < PRESSURE_JUMP_MAX) {
        // Player jump with the following mathematical function
        isJumping = true;
        moveY = -(float) (2 * timeSpentJumping - 0.3 * (timeSpentJumping * timeSpentJumping));
        timeSpentJumping += 0.1F;
    } else if (timeSpentJumping > 0 && timeSpentJumping < PRESSURE_JUMP_MIN) {
        moveY = -(float) (2 * timeSpentJumping - 0.3 * (timeSpentJumping * timeSpentJumping));
        timeSpentJumping += 0.1F;
    }
        // The player doesn't jump
    else {
        timeSpentJumping = 0;
        wantToJump = false;

        // The player is on a platform
        if (isOnPlatform) {
            moveY = 0;
            isJumping = false;
            timeAfterFall = COYOTE_TIME;
        }
            // The player is falling
        else {
            moveY = (timeAfterFall > 0) ? 2 - timeAfterFall : 2;
            timeAfterFall -= 0.1F;
        }
    }
}

void Player::render(SDL_Renderer *renderer, Point camera) {
    sprite.updateAnimation(); // Update sprite animation
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
    std::vector<Point> vertexRight = getVerticesRight();
    for (size_t i = 0; i < vertexRight.size(); ++i) {
        const auto &vertex1 = vertexRight[i];
        const auto &vertex2 = vertexRight[(i + 1) % vertexRight.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the left collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    std::vector<Point> vertexLeft = getVerticesLeft();
    for (size_t i = 0; i < vertexLeft.size(); ++i) {
        const auto &vertex1 = vertexLeft[i];
        const auto &vertex2 = vertexLeft[(i + 1) % vertexLeft.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the roof collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertex = getVerticesRoof();
    for (size_t i = 0; i < vertex.size(); ++i) {
        const auto &vertex1 = vertex[i];
        const auto &vertex2 = vertex[(i + 1) % vertex.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
    //Draw the ground collider
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<Point> vertexGround = getVerticesGround();
    for (size_t i = 0; i < vertexGround.size(); ++i) {
        const auto &vertex1 = vertexGround[i];
        const auto &vertex2 = vertexGround[(i + 1) % vertexGround.size()];
        SDL_RenderDrawLineF(renderer, vertex1.x - camera.x, vertex1.y - camera.y, vertex2.x - camera.x, vertex2.y - camera.y);
    }
}