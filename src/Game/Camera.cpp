#include "../../include/Game/Camera.h"

Camera::Camera() = default;
Camera::Camera(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {} /**< Constructor for the Camera class. */

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

SDL_FRect Camera::getArea() const {
    return area;
}

/** METHODS **/

void Camera::initializeCameraPosition(Point camera_point) {
    // Initialize the camera so that players are bottom left
    x = camera_point.x;
    y = camera_point.y - 2 * (h / 3);

    // The point is on the right of the area
    if (camera_point.x > x + (w - (w / 2))) {
        x += camera_point.x - (x + (w - (w / 2)));
    }
        // The point is on the left of the area
    else if (camera_point.x < x + (w / 5)) {
        x -= (x + (w / 5)) - camera_point.x;
    }
    // The point is on the bottom of the area
    if (camera_point.y > y + (h - (h / 5))) {
        y += camera_point.y - (y + (h - (h / 5)));
    }
        // The point is on the top of the area
    else if (camera_point.y < y + (h / 5)) {
        y -= (y + (h / 5)) - camera_point.y;
    }
}

void Camera::applyCameraMovement(Point camera_point) {
    // The point is on the right of the area
    if (camera_point.x > x + w - (w / 2)) {
        x += (camera_point.x - (x + (w - (w / 2)))) * LERP_SMOOTHING_FACTOR;
    }
        // The point is on the left of the area
    else if (camera_point.x < x + (w / 5)) {
        x -= ((x + (w / 5)) - camera_point.x) * LERP_SMOOTHING_FACTOR;
    }
    // The point is on the bottom of the area
    if (camera_point.y > y + (h - (h / 5))) {
        y += (camera_point.y - (y + (h - (h / 5)))) * LERP_SMOOTHING_FACTOR;
    }
        // The point is on the top of the area
    else if (camera_point.y < y + (h / 5)) {
        y -= ((y + (h / 5)) - camera_point.y) * LERP_SMOOTHING_FACTOR;
    }
}


