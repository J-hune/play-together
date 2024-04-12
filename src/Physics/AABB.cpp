#include "../../include/Physics/AABB.h"

/**
 * @file AABB.cpp
 * @brief Implements the Axis-Aligned Bounding Box class responsible for handling AABBs in 2D space.
 */

/* CONSTRUCTOR */

AABB::AABB(float x, float y, float width, float height, AABBType type) : x(x), y(y), width(width), height(height), type(type) {}


/* ACCESSORS */

float AABB::getX() const {
    return x;
}

float AABB::getY() const {
    return y;
}

float AABB::getWidth() const {
    return width;
}

float AABB::getHeight() const {
    return height;
}

AABBType AABB::getType() const {
    return type;
}

SDL_FRect AABB::getRect() const {
    return {x, y, width, height};
}


/* MODIFIERS */

void AABB::setX(float value) {
    AABB::x = value;
}

void AABB::setY(float value) {
    AABB::y = value;
}

void AABB::setWidth(float value) {
    AABB::width = value;
}

void AABB::setHeight(float value) {
    AABB::height = value;
}

void AABB::setType(AABBType value) {
    AABB::type = value;
}

void AABB::setPosition(const Point &position) {
    x = position.x;
    y = position.y;
}