#include "../../include/Game/Camera.h"

/**
 * @file Camera.cpp
 * @brief Implements the Camera class responsible for handling camera logic.
 */

/** CONSTRUCTORS **/

Camera::Camera() = default;


/** ACCESSORS **/

float Camera::getX() const {
    return x;
}

float Camera::getY() const {
    return y;
}

float Camera::getW() const {
    return w;
}

float Camera::getH() const {
    return h;
}

SDL_FRect Camera::getBroadPhaseArea() const {
    return {x - 500, y - 500, w + 1000, h + 1000};
}

std::vector<Point> Camera::getBroadPhaseAreaVertices() const {
    return {
            {x - 500, y - 500},
            {x + w + 1000, y - 500},
            {x + w + 1000, y + h + 1000},
            {x - 500, y + h + 1000}
    };
}


/** MODIFIERS **/

void Camera::setX(float val) {
    x = val;
}

void Camera::setY(float val) {
    y = val;
}

void Camera::setIsShaking(bool state) {
    isShaking = state;
}

void Camera::toggleIsShaking() {
    isShaking = !isShaking;
}


/** METHODS **/

void Camera::initializePosition(Point camera_point) {
    // Initialize the camera so that players are bottom left
    x = camera_point.x;
    y = camera_point.y - 2 * (h / 3);

    // The point is on the right of the area
    if (camera_point.x > x + area.x + area.w) {
        x += camera_point.x - (x + area.x + area.w);
    }
    // The point is on the left of the area
    else if (camera_point.x < x + area.x) {
        x -= (x + area.x) - camera_point.x;
    }
    // The point is on the bottom of the area
    if (camera_point.y > y + area.y + area.h) {
        y += camera_point.y - (y + area.y + area.h);
    }
    // The point is on the top of the area
    else if (camera_point.y < y + area.y) {
        y -= (y + area.y) - camera_point.y;
    }
}

void Camera::applyMovement(Point camera_point, double deltaTime) {
    float blend = 1 - std::pow(0.5F, deltaTime * lerpSmoothingFactor);

    float area_left = x + area.x;
    float area_right = x + area.x + area.w;
    float area_top = y + area.y;
    float area_bottom = y + area.y + area.h;

    // "+ 0.1F" to stop infinite lerp smoothing !

    // The point is on the right of the area
    if (camera_point.x > area_right) {
        x += (camera_point.x - area_right) * blend + 0.1F;
    }
    // The point is on the left of the area
    else if (camera_point.x < area_left) {
        x += (camera_point.x - area_left) * blend - 0.1F;
    }
    // The point is on the bottom of the area
    if (camera_point.y > area_bottom) {
        y += (camera_point.y - area_bottom) * blend + 0.1F;
    }
    // The point is on the top of the area
    else if (camera_point.y < area_top) {
        y += (camera_point.y - area_top) * blend - 0.1F;
    }

    if (isShaking) makeCameraShake();
}

void Camera::makeCameraShake() {
    // Creating a random number generator
    std::random_device rd;
    std::minstd_rand gen(rd()); // Using the Linear Congruential Generator

    // Defining uniform distributions for generating random boolean and float values
    std::uniform_int_distribution randBool(0, 1);
    std::uniform_real_distribution<float> randFloat(0.0, 1.0);

    // Calculating target offsets for camera shake
    float targetX = randFloat(gen) * shakeAmplitude;
    float targetY = randFloat(gen) * shakeAmplitude;

    // Applying camera shake by randomly adding or subtracting target offsets
    randBool(gen) ? x += targetX : x -= targetX;
    randBool(gen) ? y += targetY : y -= targetY;
}

void Camera::renderCameraPoint(SDL_Renderer *renderer, Point camera_point) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_FRect cameraPointRect = {camera_point.x - x, camera_point.y - y, 20, 20};
    SDL_RenderFillRectF(renderer, &cameraPointRect);
    SDL_RenderDrawRectF(renderer, &cameraPointRect);
}

void Camera::renderCameraArea(SDL_Renderer *renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_FRect cameraRect = {area.x, area.y, area.w + 20, area.h + 30};
    SDL_RenderDrawRectF(renderer, &cameraRect);
}