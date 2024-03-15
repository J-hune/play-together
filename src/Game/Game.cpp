#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, const Player &initialPlayer, bool *quitFlag)
        : window(window), renderer(renderer), camera(camera), level(std::move(level)), initialPlayer(initialPlayer), quitFlagPtr(quitFlag) {}

Game::Game() :
            window(SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN)),
            renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
            level("experimentation") {}


/** ACCESSORS **/

GameState Game::getGameState() const {
    return gameState;
}

Camera* Game::getCamera() {
    return &camera;
}

Point Game::getAveragePlayersPositions() const {
    float i = 1;  // Number of player in the game (at least one)
    float x = initialPlayer.getX(); // Initialization of the point on the initial player
    float y = initialPlayer.getY();

    // Add x and y position of all players
    for (const Player &character : characters) {
        x += character.getX();
        y += character.getY();
        i++;
    }

    // Average x and y position of all players
    x /= i; y /= i;

    return {x, y};
}

Player &Game::getPlayer() {
    return initialPlayer;
}

std::vector<Player> &Game::getCharacters() {
    return characters;
}


/** MODIFIERS **/

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name);
}

void Game::setRenderCameraPoint(bool state){
    render_camera_point = state;
}

void Game::setRenderCameraArea(bool state){
    render_camera_area = state;
}

void Game::setRenderPlayerColliders(bool state) {
    render_player_colliders = state;
}

void Game::setEnablePlatformsMovement(bool state) {
    enable_platforms_movement = state;
}

void Game::toggleRenderTextures() {
    render_textures = !render_textures;
}


/** FUNCTIONS **/

// Function to check AABB collision between two rectangles
bool checkAABBCollision(const SDL_FRect &a, const SDL_FRect &b) {
    // Check for AABB collision
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}


/** METHODS **/

Player* Game::findPlayerById (int id) {
    for (Player &character : characters) {
        if (character.getPlayerID() == id) {
            return &character;
        }
    }
    return nullptr;
}

void Game::teleportPlayer(float newX, float newY) {
    initialPlayer.teleportPlayer(newX, newY);
}

void Game::addCharacter(const Player &character) {
    characters.push_back(character);
}

void Game::removeCharacter(Player* characterPtr) {
    // Check if the pointer is valid
    if (characterPtr == nullptr) {
        return; // Exit if the pointer is null
    }

    // Find the iterator corresponding to the character in the vector
    auto it = std::find_if(characters.begin(), characters.end(), [characterPtr](const Player& currentPlayer) {
        return &currentPlayer == characterPtr;
    });

    // Check if the character was found
    if (it != characters.end()) {
        // Erase the character from the vector
        characters.erase(it);
    }
}


void Game::handleKeyDownEvent(Player *player, const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // If the coyote time is passed and the player is not already in a jump
            if (player->getTimeAfterFall() > 0 && !player->getIsJumping()) {
                player->setWantToJump(true);
            }
            break;
        case SDLK_LEFT:
        case SDLK_q:
            player->setDesiredDirection(PLAYER_LEFT);
            player->setWantToMoveLeft(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_HORIZONTAL);
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            player->setDesiredDirection(PLAYER_RIGHT);
            player->setWantToMoveRight(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_NONE);
            break;
        case SDLK_g:
            switchGravity = !switchGravity;
            player->setIsOnPlatform(false);
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
            player->getSprite()->toggleFlipVertical();
            break;
        case SDLK_m:
            printf("Loading map 'diversity'\n");
            setLevel("diversity");
            break;
        case SDLK_ESCAPE:
            pause();
            break;
        case SDLK_DELETE:
            stop();
            break;
        default:
            break;
    }
}

void Game::handleKeyUpEvent(Player *player, const SDL_KeyboardEvent& keyEvent) {
    switch (keyEvent.keysym.sym) {
        case SDLK_UP:
        case SDLK_z:
        case SDLK_SPACE:
            // Reset vertical movement if moving upwards
            if (player->getMoveY() < 0) player->setMoveY(0);
            player->setWantToJump(false); // Disable jumping
            break;
        case SDLK_DOWN:
            // Reset vertical movement if moving downwards
            if (player->getMoveY() > 0) player->setMoveY(0);
        case SDLK_q:
        case SDLK_LEFT:
            // Reset horizontal movement if moving left
            if (player->getMoveX() < 0) player->setMoveX(0);
            player->setWantToMoveLeft(false); // Disable left movement
            // Trigger idle animation if not moving right
            if (!player->getWantToMoveRight()) {
                player->setFinishTheMovement(false);
                player->getSprite()->setAnimation(Player::idle);
            }
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            // Reset horizontal movement if moving right
            if (player->getMoveX() > 0) {
                player->setMoveX(0);
            }
            player->setWantToMoveRight(false); // Disable right movement
            // Trigger idle animation if not moving left
            if (!player->getWantToMoveLeft()) {
                player->setFinishTheMovement(false);
                player->getSprite()->setAnimation(Player::idle);
            }
            break;
        default:
            break;
    }
}

void Game::handleEvents(Player *player) {
    SDL_Event e;
    static uint16_t lastKeyboardStateMask = 0;

    // Main loop handling every event one by one
    while (SDL_PollEvent(&e) != 0) {
        // Handle SDL_QUIT event
        if (e.type == SDL_QUIT) {
            printf("Quit event detected\n");

            *quitFlagPtr = true;
            stop();
        }

        // Handle key events
        if (e.type == SDL_KEYUP) {
            handleKeyUpEvent(player, e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(player, e.key);
        }

            // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }

    // Send the keyboard state to the network after handling all events
    sendKeyboardStateToNetwork(&lastKeyboardStateMask);
}


void Game::applyPlayerMovement(Player *player) {
    // If the gravity is switched change the direction of the y-axis move
    if (switchGravity) {
        player->setMoveY(player->getMoveY()* -1);
    }

    // If the player can't move on x-axis, don't apply the movement
    if(!player->getCanMove()){
        player->setMoveX(0);
    }
    if(player->getX() < camera.getX()){
        player->setX(camera.getX());
    }
    else if(player->getX() + player->getW() > camera.getX() + camera.getW()) {
        player->setX(camera.getX() + camera.getW() - player->getW());
    }
    else{
        player->setX(player->getX() + player->getMoveX());
    }
    player->setY(player->getY() + player->getMoveY());
}

void Game::applyAllPlayerMovement() {
    applyPlayerMovement(&initialPlayer); // Apply movement for the initial player

    // Apply movement for other players
    for (Player &character : characters) {
        applyPlayerMovement(&character);
    }
}

void Game::calculateAllPlayerMovement() {
    initialPlayer.calculateMovement(); // Calculate movement for the initial player

    // Apply movement for other players
    for (Player &character : characters) {
        character.calculateMovement();
    }
}


/** HANDLE COLLISIONS **/

void Game::handleCollisionsWithObstacles(Player *player) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: level.getObstacles()) {
        // If collision detected with the roof, the player can't jump anymore
        if (checkCollision(player->getVerticesRoof(), obstacle)) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the ground, the player is on a platform
        if (!player->getIsOnPlatform() && checkCollision(player->getVerticesGround(), obstacle)) {
            player->setIsOnPlatform(true);
        }
        // If collision detected with the wall, the player can't move
        if (player->getCanMove() && checkCollision(player->getVerticesHorizontal(), obstacle)) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }
}

void Game::handleCollisionsWithPlatforms(Player *player) {
    // Check for collisions with each 1D moving platforms
    for (const MovingPlatform1D &platform: level.getMovingPlatforms1D()) {
        // If collision detected with the roof, the player can't jump anymore
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the ground, the player is on the platform
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
            // Add platform velocity to the player by checking on which axis it moves
            platform.getAxis() ? player->setY(player->getY() + platform.getMove()) : player->setX(player->getX() + platform.getMove());
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }

    // Check for collisions with each 2D moving platforms
    for (const MovingPlatform2D &platform: level.getMovingPlatforms2D()) {
        // If collision detected with the roof, the player can't jump anymore
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the ground, the player is on the platform
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
            // Add platform velocity to the player
            player->setX(player->getX() + platform.getMoveX());
            player->setY(player->getY() + platform.getMoveY());
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }

    // Check for collisions with each switching platforms
    for (const SwitchingPlatform &platform: level.getSwitchingPlatforms()) {
        // If collision detected with the roof, the player can't jump anymore
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the ground, the player is on the platform
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }
}

void Game::handleCollisionsWithOtherPlayers(Player *player) {
    /*
    // Check for collisions with other characters
    for (Player const &character : characters) {
    if (&character != &player && checkAABBCollision(player.getBoundingBox(), character.getBoundingBox())) {
    printf("Collision detected with another character\n");
    player.x -= moveX;
    player.y -= moveY;
    }
    }
    */
}

void Game::handleCollisionsWithCameraBorders(Player *player) {
    // Check for collisions with right camera borders
    if (player->getX() > camera.getX() + camera.getW() - player->getW()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX()/5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the left camera borders
        for (Player &character : characters){
            if(character.getX() < camera.getX()){
                character.setX(player->getMoveX());
            }
        }
    }
    // Check for collisions with left camera borders
    else if (player->getX() < camera.getX()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX()/5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the right camera borders
        for (Player &character: characters) {
            if (character.getX() > camera.getX() + camera.getW() - character.getW()) {
                character.setX(player->getMoveX());
            }
        }
    }
    /*// Check for collisions with down camera borders
    * if (player.y < camera.y){
    *
    * }
    */
    /*// Check for collisions with up camera borders
    * if (player.y > camera.y + camera.h - player.height){
    *
    * }
    */
}


void Game::handleCollisions() {

    // Check collisions for the initial player
    this->initialPlayer.setCanMove(true);

    // Check obstacles collisions only if the player has moved
    if (this->initialPlayer.getMoveY() != 0 || this->initialPlayer.getCurrentDirection() != 0) {
        this->initialPlayer.setIsOnPlatform(false);
        handleCollisionsWithObstacles(&this->initialPlayer);
    }

    handleCollisionsWithOtherPlayers(&this->initialPlayer); // Check collisions with other players
    handleCollisionsWithPlatforms(&this->initialPlayer); // Check collisions with platforms
    handleCollisionsWithCameraBorders(&this->initialPlayer); // Check collisions with camera borders

    // Check collisions for other players
    for (Player &character : characters) {
        character.setCanMove(true);

        // Check obstacles collisions only if the player has moved
        if (character.getMoveY() != 0 || character.getCurrentDirection() != 0) {
            character.setIsOnPlatform(false);
            handleCollisionsWithObstacles(&character);
        }

        handleCollisionsWithOtherPlayers(&character); // Check collisions with other players
        handleCollisionsWithPlatforms(&character); // Check collisions with platforms
        handleCollisionsWithCameraBorders(&character); // Check collisions with camera borders
    }
}


/** HANDLE COLLISIONS REVERSED MAVITY **/

void Game::handleCollisionsWithObstaclesReverseMavity(Player *player) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: level.getObstacles()) {
        // If collision detected with the roof, the player is on a platform
        if (checkCollision(player->getVerticesRoof(), obstacle)) {
            player->setIsOnPlatform(true);
        }
        // If collision detected with the ground, the player can't jump anymore
        if (!player->getIsOnPlatform() && checkCollision(player->getVerticesGround(), obstacle)) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the wall, the player can't move
        if (player->getCanMove() && checkCollision(player->getVerticesHorizontal(), obstacle)) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }
}

void Game::handleCollisionsWithPlatformsReversedMavity(Player *player) {
    // Check for collisions with each 1D moving platforms
    for (const MovingPlatform1D &platform: level.getMovingPlatforms1D()) {
        // If collision detected with the roof, the player is on the platform
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
            // Add platform velocity to the player by checking on which axis it moves
            platform.getAxis() ? player->setY(player->getY() + platform.getMove()) : player->setX(player->getX() + platform.getMove());
        }
        // If collision detected with the ground, the player can't jump anymore
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }

    // Check for collisions with each 2D moving platforms
    for (const MovingPlatform2D &platform: level.getMovingPlatforms2D()) {
        // If collision detected with the roof, the player is on the platform
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
            // Add platform velocity to the player
            player->setX(player->getX() + platform.getMoveX());
            player->setY(player->getY() + platform.getMoveY());
        }
        // If collision detected with the ground, the player can't jump anymore
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }

    // Check for collisions with each switching platforms
    for (const SwitchingPlatform &platform: level.getSwitchingPlatforms()) {
        // If collision detected with the roof, the player is on the platform
        if (checkAABBCollision(player->getRoofColliderBoundingBox(), platform.getBoundingBox())) {
            player->setIsOnPlatform(true);
        }
        // If collision detected with the ground, the player can't jump anymore
        if (checkAABBCollision(player->getGroundColliderBoundingBox(), platform.getBoundingBox())) {
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
        }
        // If collision detected with the wall, the player can't move
        if (checkAABBCollision(player->getHorizontalColliderBoundingBox(), platform.getBoundingBox())) {
            player->setCanMove(false);
            player->setTimeSpeed(0);
        }
    }
}

void Game::handleCollisionsWithOtherPlayersReversedMavity(Player *player) {
    /*
    // Check for collisions with other characters
    for (Player const &character : characters) {
    if (&character != &player && checkAABBCollision(player.getBoundingBox(), character.getBoundingBox())) {
    printf("Collision detected with another character\n");
    player.x -= moveX;
    player.y -= moveY;
    }
    }
    */
}

void Game::handleCollisionsWithCameraBordersReversedMavity(Player *player) {
    // Check for collisions with right camera borders
    if (player->getX() > camera.getX() + camera.getW() - player->getW()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX() / 5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the left camera borders
        for (Player &character : characters){
            if(character.getX() < camera.getX()){
                character.setX(player->getMoveX());
            }
        }
    }
    // Check for collisions with left camera borders
    else if (player->getX() < camera.getX()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX() / 5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the right camera borders
        for (Player &character: characters) {
            if (character.getX() > camera.getX() + camera.getW() - character.getW()) {
                character.setX(player->getMoveX());
            }
        }
    }
    /*// Check for collisions with down camera borders
    * if (player.y < camera.y){
    *
    * }
    */
    /*// Check for collisions with up camera borders
    * if (player.y > camera.y + camera.h - player.height){
    *
    * }
    */
}

void Game::handleCollisionsReversedMavity() {

    // Check collisions for the initial player
    this->initialPlayer.setCanMove(true);

    // Check obstacles collisions only if the player has moved
    if (this->initialPlayer.getMoveY() != 0 || this->initialPlayer.getCurrentDirection() != 0) {
        this->initialPlayer.setIsOnPlatform(false);
        handleCollisionsWithObstaclesReverseMavity(&this->initialPlayer);
    }

    handleCollisionsWithOtherPlayersReversedMavity(&this->initialPlayer); // Check collisions with other players
    handleCollisionsWithPlatformsReversedMavity(&this->initialPlayer); // Check collisions with platforms
    handleCollisionsWithCameraBordersReversedMavity(&this->initialPlayer); // Check collisions with camera borders

    // Check collisions for other players
    for (Player &character : characters) {
        character.setCanMove(true);

        // Check obstacles collisions only if the player has moved
        if (character.getMoveY() != 0 || character.getCurrentDirection() != 0) {
            character.setIsOnPlatform(false);
            handleCollisionsWithObstaclesReverseMavity(&character);
        }

        handleCollisionsWithOtherPlayersReversedMavity(&character); // Check collisions with other players
        handleCollisionsWithPlatformsReversedMavity(&character); // Check collisions with platforms
        handleCollisionsWithCameraBordersReversedMavity(&character); // Check collisions with camera borders
    }
}

void Game::render() {
    Point cam = {camera.getX(), camera.getY()};

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render textures
    if (render_textures) {
        initialPlayer.render(renderer, cam); // Draw the initial player

        // Draw the characters
        for (Player &character: characters) {
            character.render(renderer, cam);
        }

        level.renderObstaclesDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }
    // Render collisions box
    else {
        initialPlayer.renderDebug(renderer, cam); // Draw the initial player

        // Draw the characters
        for (Player const &character: characters) {
            character.renderDebug(renderer, cam);
        }

        level.renderObstaclesDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }

    // DEBUG DRAWING OF APPLICATION CONSOLE :

    // Draw the camera point if enabled
    if (render_camera_point) camera.renderCameraPoint(renderer, getAveragePlayersPositions());

    // Draw the camera area if enabled
    if (render_camera_area) camera.renderCameraArea(renderer);

    // Draw the player's colliders if enabled
    if (render_player_colliders) initialPlayer.renderColliders(renderer, cam);

    // Present the renderer and introduce a slight delay
    SDL_RenderPresent(renderer);
    SDL_Delay(4);
}

bool Game::checkCollision(const std::vector<Point> &playerVertices, const Polygon &obstacle) {
    // Check for convexity of the obstacle
    if (!obstacle.isConvex()) {
        printf("The obstacle is not convex\n");
        return false;
    }

    // Potential separation axes (normals to the sides of the rectangle)
    std::vector<Point> axes = {
            {1, 0},
            {0, 1}
    };

    // Add axes perpendicular to the sides of the polygon
    for (size_t i = 0; i < obstacle.getVertices().size(); ++i) {
        std::vector<Point> vertices = obstacle.getVertices();
        const Point &vertex1 = vertices[i];
        const Point &vertex2 = vertices[(i + 1) % vertices.size()];

        Point edge = {vertex2.x - vertex1.x, vertex2.y - vertex1.y};
        Point axis = {-edge.y, edge.x};
        axes.push_back(axis);
    }

    for (Point axis: axes) {
        // Project the rectangle and the polygon onto the axis
        auto playerProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto playerProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: playerVertices) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            playerProjectionMin = std::min(playerProjectionMin, projection);
            playerProjectionMax = std::max(playerProjectionMax, projection);
        }

        auto obstacleProjectionMin = static_cast<float>(std::numeric_limits<int>::max());
        auto obstacleProjectionMax = static_cast<float>(std::numeric_limits<int>::min());

        for (const Point &vertex: obstacle.getVertices()) {
            float projection = vertex.x * axis.x + vertex.y * axis.y;
            obstacleProjectionMin = std::min(obstacleProjectionMin, projection);
            obstacleProjectionMax = std::max(obstacleProjectionMax, projection);
        }

        // Check for separation on the axis
        if (playerProjectionMax < obstacleProjectionMin || obstacleProjectionMax < playerProjectionMin) {
            return false; // No collision detected
        }
    }

    return true; // Collision detected
}

void Game::run() {
    gameState = GameState::RUNNING;

    // Game loop
    while (gameState == GameState::RUNNING) {
        // Handle events, calculate player movement, check collisions, apply player movement, apply camera movement and render
        handleEvents(&initialPlayer);
        for (Player &character : characters) handleEvents(&character);
        if (enable_platforms_movement) level.applyPlatformsMovement();
        calculateAllPlayerMovement();
        switchGravity ? handleCollisionsReversedMavity() : handleCollisions();
        applyAllPlayerMovement();
        camera.applyCameraMovement(getAveragePlayersPositions());
        render();
    }
}

void Game::pause() {
    gameState = GameState::PAUSED;
}

void Game::stop() {
    gameState = GameState::STOPPED;

    // Reset the player position
    initialPlayer.setX(50);
    initialPlayer.setY(50);
}


/** STATIC METHODS **/

void Game::sendKeyboardStateToNetwork(uint16_t *lastKeyboardStateMaskPtr) {

    // Get the current keyboard state mask
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    uint16_t currentKeyboardStateMask = Mediator::encodeKeyboardStateMask(keyboardState);

    // If the keyboard state has changed since the last message was sent
    if (currentKeyboardStateMask != *lastKeyboardStateMaskPtr) {
        // Send the new keyboard state mask to the server
        Mediator::sendPlayerUpdate(currentKeyboardStateMask);

        // Update the last keyboard state mask and the last send time
        *lastKeyboardStateMaskPtr = currentKeyboardStateMask;
    }
}
