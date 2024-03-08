#include "../../../include/Game/Objects/Player.h"

/**
 * @file Player.cpp
 * @brief Implements the Player class representing a player in a 2D game.
 */


/** CONSTRUCTOR **/

Player::Player(float startX, float startY, float playerSpeed,float speedMax, float playerWidth, float playerHeight)
        : x(startX), y(startY), speed(playerSpeed), speedMax(speedMax), width(playerWidth), height(playerHeight){}


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

float Player::getTimeAfterFall() const {
    return timeAfterFall;
}

int Player::getCurrentDirection() const {
    return currentDirection;
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

std::vector<Point> Player::getVerticesHorizontal(int direction) const {
    // Return the vertices of the player's bounding box, with added margin to capture wall within the area.

    std::vector<Point> verticesDirection;

    if(direction==PLAYER_LEFT){
        verticesDirection = getVerticesLeft();
    }
    else{
        verticesDirection = getVerticesRight();
    }

    return verticesDirection;
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

void Player::setFinishTheMovement(bool state){
    finishTheMovement = state;
}

void Player::setTimeSpeed(float val){
    timeSpeed = val;
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

void Player::calculatePlayerMovement(float &moveX, int direction, float &moveY) {

    // The player move on the x-axis
    if (finishTheMovement && (wantToMoveLeft || wantToMoveRight)) {
        timeSpeed += 0.1F;
        moveX = speedMax * speed * timeSpeed;
        if (moveX > speedMax) {
            moveX = speedMax;
            timeSpeed = maxSpeedReachWithThisTime;
        }
        moveX *= (float)direction;
        currentDirection = direction;
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