#include <cmath> // Include math functions
#include <random> // Include random number generation functions
#include "../../../include/Game/Events/Asteroid.h" // Include the Asteroid class header file

// Static member initialization
SDL_Texture *Asteroid::spriteTexturePtr = nullptr;
std::vector<float> Asteroid::angles;
std::vector<int> Asteroid::anglesLock;
std::vector<int> Asteroid::positionsLock;
std::vector<float> Asteroid::positions;

/** CONSTRUCTORS **/

// Constructor for Asteroid class with default parameters
Asteroid::Asteroid(float x, float y): x(x + Asteroid::getRandomPosition()), y(y - 60), speed(0.6f) {
    angle = getRandomAngle();
    sprite = Sprite(Asteroid::idle, *spriteTexturePtr, 64, 64); // Initialize sprite with default animation
}

// Constructor for Asteroid class with specified parameters
Asteroid::Asteroid(float x, float y, float speed, float h, float w, float angle)
        : x(x), y(y), h(h), w(w), speed(speed), angle(angle) {
    sprite = Sprite(Asteroid::idle, *spriteTexturePtr, 64, 64); // Initialize sprite with default animation
}


/** BASIC ACCESSORS **/

// Return the x attribute
float Asteroid::getX() const {
    return x;
}

// Return the y attribute
float Asteroid::getY() const {
    return y;
}

// Return the width attribute
float Asteroid::getW() const {
    return w;
}

// Return the height attribute
float Asteroid::getH() const {
    return h;
}

// Return the angle attribute
float Asteroid::getAngle() const {
    return angle;
}

// Return a pointer to the sprite attribute
Sprite* Asteroid::getSprite() {
    return &sprite;
}

// Return the speed attribute
float Asteroid::getSpeed() const {
    return speed;
}


/** SPECIFIC ACCESSORS **/

// Return the bounding box of the asteroid
SDL_FRect Asteroid::getBoundingBox() const {
    return {x, y, w, h};
}

// Get the vertices of the asteroid's bounding box
std::vector<Point> Asteroid::getVertices() const {
    return {
            {x, y},
            {x + w, y},
            {x + w, y + h},
            {x, y + h}
    };
}


/** MODIFIERS **/

// Set the x attribute
void Asteroid::setX(float val) {
    x = val;
}

// Set the y attribute
void Asteroid::setY(float val) {
    y = val;
}

// Set the width attribute
void Asteroid::setW(float val) {
    w = val;
}

// Set the height attribute
void Asteroid::setH(float val) {
    h = val;
}

// Set the angle attribute
void Asteroid::setAngle(float val) {
    angle = val;
}


/** METHODS **/

// Load textures for the asteroid
bool Asteroid::loadTextures(SDL_Renderer &renderer) {
    // Load asteroid sprite texture
    spriteTexturePtr = IMG_LoadTexture(&renderer, "assets/sprites/asteroid/asteroid.png");

    // Check for errors
    if (spriteTexturePtr == nullptr) {
        return false; // Return failure
    }

    return true; // Return success
}

// Render the asteroid sprite
void Asteroid::render(SDL_Renderer *renderer, Point camera) {
    sprite.updateAnimation(); // Update sprite animation
    SDL_Rect srcRect = sprite.getSrcRect();
    SDL_FRect asteroidRect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderCopyExF(renderer, sprite.getTexture(), &srcRect, &asteroidRect, angle, nullptr, sprite.getFlip());
}

// Render the asteroid collision box
void Asteroid::renderDebug(SDL_Renderer *renderer, Point camera) const {
    SDL_SetRenderDrawColor(renderer, 173, 79, 9, 255);
    SDL_FRect asteroidRect = {x - camera.x, y - camera.y, w, h};
    SDL_RenderFillRectF(renderer, &asteroidRect);
}

// Apply asteroid movement based on speed and angle
void Asteroid::applyMovement(double deltaTime) {
    angle_radians = static_cast<float>(angle * (M_PI / 180)); // Convert angle to radians

    // Calculate horizontal and vertical speeds
    horizontalSpeed = - speed * std::cos(angle_radians);
    verticalSpeed = - speed * std::sin(angle_radians);

    // Update current coordinates based on speed components
    x += 100 * horizontalSpeed * static_cast<float>(deltaTime); // '* 100' temporaire (c'est pour qu'il avance plus vite)
    y += 100 * verticalSpeed * static_cast<float>(deltaTime);
}

// Generate an array of possible positions for the asteroid
int Asteroid::generateArrayPositions(int nbPosition, float x, float y, int seed) {
    // If no seed is provided, generate a random seed
    if (seed == 0){
        std::random_device rd;
        seed = rd();
    }
    std::mt19937 gen(seed); // Initialize random number generator with seed
    std::uniform_real_distribution<float> dis(x, y); // Define uniform distribution

    // Reserve space for vectors to avoid frequent reallocation
    positions.reserve(nbPosition);
    positionsLock.reserve(nbPosition);

    // Generate positions and mark them as unlocked
    for (int i = 0; i < nbPosition; i++) {
        float random_position = dis(gen);
        positions.push_back(random_position);
        positionsLock.push_back(0);
    }

    return seed; // Return the seed used for random number generation
}

// Generate a random position from the array of possible positions
float Asteroid::getRandomPosition(int seed) {
    // If no seed is provided, generate a random seed
    if (seed == 0){
        std::random_device rd;
        seed = rd();
    }
    std::mt19937 gen(seed); // Initialize random number generator with seed
    std::uniform_int_distribution<int> dis(0, positionsLock.size()); // Define uniform distribution

    int random_index = dis(gen);
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
    for(int & positionLock : positionsLock){
        positionLock = positionLock - 1 >= 0 ? positionLock - 1 : positionLock;
    }

    return res; // Return the randomly selected position
}

// Generate an array of possible angles for the asteroid
int Asteroid::generateArrayAngles(int nbAngle, int seed) {
    // If no seed is provided, generate a random seed
    if (seed == 0){
        std::random_device rd;
        seed = rd();
    }
    std::mt19937 gen(seed); // Initialize random number generator with seed
    std::uniform_real_distribution<float> dis(210.0f, 330.0f); // Define uniform distribution

    // Reserve space for vectors to avoid frequent reallocation
    angles.reserve(nbAngle);
    anglesLock.reserve(nbAngle);

    // Generate angles and mark them as unlocked
    for (int i = 0; i < nbAngle; i++){
        float random_angle = dis(gen);
        angles.push_back(random_angle);
        anglesLock.push_back(0);
    }

    return seed; // Return the seed used for random number generation
}

// Generate a random angle from the array of possible angles
float Asteroid::getRandomAngle(int seed) {
    // If no seed is provided, generate a random seed
    if (seed == 0){
        std::random_device rd;
        seed = rd();
    }
    std::mt19937 gen(seed); // Initialize random number generator with seed
    std::uniform_int_distribution<int> dis(0, anglesLock.size()); // Define uniform distribution

    int random_index = dis(gen);
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
    for(int & angleLock : anglesLock){
        angleLock = angleLock - 1 >= 0 ? angleLock - 1 : angleLock;
    }

    return res; // Return the randomly selected angle
}

// Trigger the explosion effect for the asteroid
void Asteroid::explode() {
    // Placeholder for explosion effect
    // angle = 0;
    // sprite.setAnimation(explosion);
}
