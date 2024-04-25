#include "../../../include/Game/Events/Rescue.h"


/**
 * @file Rescue.cpp
 * @brief Implements the Rescue class responsible for rescue zone.
 */


/* CONSTRUCTORS */

Rescue::Rescue(float x, float y, float w, float h) : x(x), y(y), w(w), h(h){}


/* ACCESSORS */

float Rescue::getX() const {
    return x;

}
float Rescue::getY() const {
    return y;
}

float Rescue::getW() const {
    return w;
}

float Rescue::getH() const {
    return h;
}

float Rescue::getNextPosition() {
    currentSpawn += spawnStepDistance;
    if (currentSpawn >= x + spawnStepDistance * 4) currentSpawn = x;
    return currentSpawn;
}

SDL_FRect Rescue::getBoundingBox() const {
    return {x, y, w, h};
}


/* MODIFIERS */

bool Rescue::setZone(AABB zone) {
    if (zone.getID() != id) {
        id = zone.getID();
        x = zone.getX();
        y = zone.getY();
        w = zone.getWidth();
        h = zone.getHeight();
        currentSpawn = x + spawnStepDistance * 4;
        return true;
    } else {
        return false;
    }
}