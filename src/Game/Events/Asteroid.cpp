#include "../../../include/Game/Events/Asteroid.h"

// Static member initialization
SDL_Texture *Asteroid::spriteTexturePtr = nullptr;
std::vector<float> Asteroid::angles;
std::vector<int> Asteroid::anglesLock;
std::vector<int> Asteroid::positionsLock;
std::vector<float> Asteroid::positions;

/** CONSTRUCTORS **/

// Constructor for Asteroid class with default parameters
Asteroid::Asteroid(float x, float y, size_t seed): x(x + Asteroid::getRandomPosition(seed)), y(y - 60), speed(0.6f), explosionSound("Events/explosion.wav") {
    angle = getRandomAngle(seed);
    sprite = Sprite(Asteroid::idle, *spriteTexturePtr, 64, 64); // Initialize sprite with default animation
}

// Constructor for Asteroid class with specified parameters
Asteroid::Asteroid(float x, float y, float speed, float h, float w, float angle)
        : x(x), y(y), h(h), w(w), speed(speed), angle(angle), explosionSound("Events/explosion.wav") {
    sprite = Sprite(Asteroid::idle, *spriteTexturePtr, 64, 64); // Initialize sprite with default animation
}


/** BASIC ACCESSORS **/

float Asteroid::getX() const {
    return x;
}

float Asteroid::getY() const {
    return y;
}

float Asteroid::getW() const {
    return w;
}

float Asteroid::getH() const {
    return h;
}

float Asteroid::getAngle() const {
    return angle;
}

Sprite* Asteroid::getSprite() {
    return &sprite;
}

float Asteroid::getSpeed() const {
    return speed;
}


/** SPECIFIC ACCESSORS **/

SDL_FRect Asteroid::getBoundingBox() const {
    return {x, y, w, h};
}

std::vector<Point> Asteroid::getVertices() const {
    return {
            {x, y},
            {x + w, y},
            {x + w, y + h},
            {x, y + h}
    };
}


/** MODIFIERS **/

void Asteroid::setX(float val) {
    x = val;
}

void Asteroid::setY(float val) {
    y = val;
}

void Asteroid::setW(float val) {
    w = val;
}

void Asteroid::setH(float val) {
    h = val;
}

void Asteroid::setAngle(float val) {
    angle = val;
}


/** METHODS **/

bool Asteroid::loadTextures(SDL_Renderer &renderer) {
    // Load asteroid sprite texture
    spriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/asteroid/asteroid.png");

    // Check for errors
    if (spriteTexturePtr == nullptr) {
        return false; // Return failure
    }

    return true; // Return success
}

void Asteroid::render(SDL_Renderer *renderer, Point camera) {
    sprite.updateAnimation(); // Update sprite animation
    SDL_Rect srcRect = sprite.getSrcRect();
    SDL_FRect asteroidRect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &asteroidRect, angle, nullptr, sprite.getFlip());
}

void Asteroid::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 173, 79, 9, 255);
    SDL_FRect asteroid_rect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &asteroid_rect);
}

void Asteroid::applyMovement(double delta_time) {
    angle_radians = static_cast<float>(angle * (M_PI / 180)); // Convert angle to radians

    // Calculate horizontal and vertical speeds
    horizontalSpeed = - speed * std::cos(angle_radians);
    verticalSpeed = - speed * std::sin(angle_radians);

    // Update current coordinates based on speed components
    x += 100 * horizontalSpeed * static_cast<float>(delta_time); // '* 100' temporary for testing TODO: remove
    y += 100 * verticalSpeed * static_cast<float>(delta_time);
}

void Asteroid::generateRandomPositionsArray(int position_count, float x, float y, size_t seed) {
    positions.clear();
    positionsLock.clear();

    std::minstd_rand gen(seed); // Initialize random number generator with seed
    std::uniform_real_distribution<float> dis(x, y); // Define uniform distribution

    // Reserve space for vectors to avoid frequent reallocation
    positions.reserve(position_count);
    positionsLock.reserve(position_count);

    // Generate positions and mark them as unlocked
    for (int i = 0; i < position_count; i++) {
        float random_position = dis(gen);
        positions.push_back(random_position);
        positionsLock.push_back(0);
    }
}

float Asteroid::getRandomPosition(size_t seed) {
    std::minstd_rand gen(seed); // Initialize random number generator with seed
    std::uniform_int_distribution<size_t> dis(0, positionsLock.size() - 1); // Define uniform distribution

    size_t random_index = dis(gen);
    float res = positions[random_index];

    // Loop until an unlocked position is found
    while(positionsLock[random_index] != 0){
        random_index = dis(gen);
    }

    // Lock the selected position
    if (positionsLock[random_index] == 0){
        res = positions[random_index];
        positionsLock[random_index] = static_cast<int> (positionsLock.size());
    }

    // Decrement all locked positions
    for(int & position_lock : positionsLock){
        position_lock = position_lock - 1 >= 0 ? position_lock - 1 : position_lock;
    }

    return res; // Return the randomly selected position
}

void Asteroid::generateRandomAnglesArray(int angle_count, size_t seed) {
    angles.clear();
    anglesLock.clear();

    std::minstd_rand gen(seed); // Initialize random number generator with seed
    std::uniform_real_distribution<float> dis(210.0f, 330.0f); // Define uniform distribution

    // Reserve space for vectors to avoid frequent reallocation
    angles.reserve(angle_count);
    anglesLock.reserve(angle_count);

    // Generate angles and mark them as unlocked
    for (int i = 0; i < angle_count; i++){
        float random_angle = dis(gen);
        angles.push_back(random_angle);
        anglesLock.push_back(0);
    }
}

float Asteroid::getRandomAngle(size_t seed) {

    std::minstd_rand gen(seed); // Initialize random number generator with seed
    std::uniform_int_distribution<size_t> dis(0, anglesLock.size() - 1); // Define uniform distribution

    size_t random_index = dis(gen);
    float res = angles[random_index];

    // Loop until an unlocked angle is found
    while(anglesLock[random_index] != 0){
        random_index = dis(gen);
    }

    // Lock the selected angle
    if (anglesLock[random_index] == 0){
        res = angles[random_index];
        anglesLock[random_index] = static_cast<int> (anglesLock.size());
    }

    // Decrement all locked angles
    for(int & angle_lock : anglesLock){
        angle_lock = angle_lock - 1 >= 0 ? angle_lock - 1 : angle_lock;
    }

    return res; // Return the randomly selected angle
}

// Trigger the explosion effect for the asteroid
void Asteroid::explode() {
    explosionSound.play(0, -1);
    // Placeholder for explosion effect
    // angle = 0;
    // sprite.setAnimation(explosion);
}
