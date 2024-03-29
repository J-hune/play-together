#include "../../include/Game/Game.h"

/**
 * @file Game.cpp
 * @brief Implements the Game class responsible for handling the main game logic.
 */

/** CONSTRUCTORS **/

Game::Game(SDL_Window *window, SDL_Renderer *renderer, const Camera &camera, Level level, bool *quitFlag)
        : window(window), renderer(renderer), camera(camera), level(std::move(level)), quitFlagPtr(quitFlag) {}

Game::Game() :
            window(SDL_CreateWindow("Play Together", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,(int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,SDL_WINDOW_SHOWN)),
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
    // Initialize sum variables
    float totalX = 0.0f;
    float totalY = 0.0f;

    // Calculate sum of x and y positions of all players
    for (const Player &character : characters) {
        totalX += character.getX();
        totalY += character.getY();
    }

    // Calculate the average by dividing by the total number of players
    size_t numPlayers = characters.size();
    if (numPlayers > 0) {
        float averageX = totalX / static_cast<float>(numPlayers);
        float averageY = totalY / static_cast<float>(numPlayers);
        return Point(averageX, averageY);
    } else {
        // If no players are present, return the origin point
        return Point(0.0f, 0.0f);
    }
}

std::vector<Player> &Game::getCharacters() {
    return characters;
}

Level &Game::getLevel() {
    return level;
}


/** MODIFIERS **/

void Game::setLevel(std::string const &map_name) {
    level = Level(map_name);
}

void Game::setRenderCameraPoint(bool state) {
    render_camera_point = state;
}

void Game::setRenderCameraArea(bool state) {
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

void Game::initialize() {
    // If the player starts the server or is playing alone
    if (!Mediator::isClientRunning()) {
        Point spawnPoint = level.getSpawnPoints()[0];

        // Add the initial player to the game
        Player initialPlayer(-1, spawnPoint, 0.2F, 2, 48, 36);
        initialPlayer.setSpriteTextureByID(2);
        addCharacter(initialPlayer);
    }
}

Player* Game::findPlayerById (int id) {
    for (Player &character : characters) {
        if (character.getPlayerID() == id) {
            return &character;
        }
    }
    return nullptr;
}

int Game::findPlayerIndexById(int id) {
    for (size_t i = 0; i < characters.size(); ++i) {
        if (characters[i].getPlayerID() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Game::addCharacter(const Player &character) {
    characters.push_back(character);
}

void Game::removeCharacter(const Player *characterPtr) {
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
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            // If the player is dead ignore
            if (player == nullptr) break;

            // If the coyote time is passed and the player is not already in a jump
            if (player->getTimeAfterFall() > 0 && !player->getIsJumping()) {
                player->setWantToJump(true);
            }
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;
            player->setDesiredDirection(PLAYER_LEFT);
            player->setWantToMoveLeft(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_HORIZONTAL);
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;
            player->setDesiredDirection(PLAYER_RIGHT);
            player->setWantToMoveRight(true);
            player->getSprite()->setAnimation(Player::walk);
            player->getSprite()->setFlipHorizontal(SDL_FLIP_NONE);
            break;
        case SDL_SCANCODE_G:
            switchGravity = !switchGravity;
            if (player == nullptr) break;
            player->setIsOnPlatform(false);
            player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
            player->getSprite()->toggleFlipVertical();
            break;
        case SDL_SCANCODE_M:
            if (level.getMapID() == 1) setLevel("assurance");
            else setLevel("diversity");
            break;
        case SDL_SCANCODE_ESCAPE:
            pause();
            break;
        case SDL_SCANCODE_DELETE:
            stop();
            break;
        default:
            break;
    }
}

void Game::handleKeyUpEvent(Player *player, const SDL_KeyboardEvent &keyEvent) const {
    switch (keyEvent.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_SPACE:
            if (player == nullptr) break;

            // Reset vertical movement if moving upwards
            if (player->getMoveY() < 0) player->setMoveY(0);
            player->setWantToJump(false); // Disable jumping
            break;
        case SDL_SCANCODE_DOWN:
            if (player == nullptr) break;

            // Reset vertical movement if moving downwards
            if (player->getMoveY() > 0) player->setMoveY(0);
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            if (player == nullptr) break;

            // Reset horizontal movement if moving left
            if (player->getMoveX() < 0) player->setMoveX(0);
            player->setWantToMoveLeft(false); // Disable left movement
            // Trigger idle animation if not moving right
            if (!player->getWantToMoveRight()) {
                player->setFinishTheMovement(false);
                player->getSprite()->setAnimation(Player::idle);
            } else {
                player->getSprite()->setFlipHorizontal(SDL_FLIP_NONE);
            }
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            if (player == nullptr) break;

            // Reset horizontal movement if moving right
            if (player->getMoveX() > 0) {
                player->setMoveX(0);
            }
            player->setWantToMoveRight(false); // Disable right movement
            // Trigger idle animation if not moving left
            if (!player->getWantToMoveLeft()) {
                player->setFinishTheMovement(false);
                player->getSprite()->setAnimation(Player::idle);
            } else {
                player->getSprite()->setFlipHorizontal(SDL_FLIP_HORIZONTAL);
            }
            break;
        default:
            break;
    }
}

void Game::handleEvents() {
    SDL_Event e;
    static uint16_t lastKeyboardStateMask = 0;
    Player *playerPtr = findPlayerById(-1);

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
            handleKeyUpEvent(playerPtr, e.key);
        }
        if (e.type == SDL_KEYDOWN) {
            handleKeyDownEvent(playerPtr, e.key);
        }

            // Handle SDL_MOUSEBUTTONDOWN events
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            printf("Mouse clicked at (%f, %f)\n", (float)e.button.x + camera.getX(), (float)e.button.y + camera.getY());
        }
    }

    // Send the keyboard state to the network after handling all events
    sendKeyboardStateToNetwork(&lastKeyboardStateMask);
}


void Game::applyPlayerMovement(Player *player) const {
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
    // Apply movement to all players
    for (Player &character : characters) {
        applyPlayerMovement(&character);
    }
}

void Game::calculateAllPlayerMovement() {
    // Apply movement to all players
    for (Player &character : characters) {
        character.calculateMovement();
    }
}

void Game::killPlayer(Player *player) {
    Point spawnPoint = level.getSpawnPoints()[findPlayerIndexById(player->getPlayerID())];

    // Add the player to the dead characters list
    deadCharacters.push_back(*player);
    characters.erase(characters.begin() + findPlayerIndexById(player->getPlayerID()));

    // Teleport the player to the spawn point
    player->teleportPlayer(spawnPoint.x, spawnPoint.y);

}


/** HANDLE COLLISIONS **/

void Game::handleCollisionsWithObstacles(Player *player) const {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: level.getZones(zoneType::COLLISION)) {
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

void Game::handleCollisionsWithDeathZone(Player *player) {
    // Check collisions with each death zone
    for (const Polygon &deathZone: level.getZones(zoneType::DEATH)) {
        // If collision detected with the death zone, kill the player
        if (checkCollision(player->getVertices(), deathZone)) {
            killPlayer(player);
        }
    }
}

void Game::handleCollisionsWithSaveZone(Player *player) {
    // Check collisions with each save zone
    const auto &saveZones = level.getZones(zoneType::SAVE);
    for (size_t i = 0; i < saveZones.size(); ++i) {
        const Polygon &saveZone = saveZones[i];        // If collision detected with the save zone, save the player (in local or server mode)
        auto savePointID = static_cast<short>(i + 1);
        if (checkCollision(player->getVertices(), saveZone) && !Mediator::isClientRunning() && level.getLastCheckpoint() < savePointID) {
            level.setLastCheckpoint(savePointID);
            std::cout << "Checkpoint reached: " << savePointID << std::endl;
        }
    }
}

void Game::handleCollisionsWithPlatforms(Player *player) const {
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
    // Get others players in a vector
    std::vector<Player*> otherCharacters;
    for (Player &character : characters) {
        if (*player != character) otherCharacters.push_back(&character);
    }

    // Check for collisions with other characters
    for (Player *character : otherCharacters) {
        if (checkAABBCollision(player->getBoundingBox(), character->getBoundingBox())) {

            //The player move to the right
            if (player->getMoveX() > 0) {
                //  The player's right is to the right of the detected player's left
                if (player->getX()+player->getW() <= character->getX() + 2) {
                    // Slow down the player's speed
                    player->setMoveX(player->getMoveX() / 5);
                    character->setMoveX(player->getMoveX());
                }
            }
                //The player move to the left
            else if (player->getMoveX() < 0) {
                if (player->getX() + 2 > character->getX()+character->getW()) {
                    player->setMoveX(player->getMoveX() / 5);
                    character->setMoveX(player->getMoveX());
                }
            }

            // If above the detected player
            if (!player->getIsOnPlatform()
               //The player's bottom is below the detected player's head
               && player->getY()+player->getH()>character->getY()
               // The player's head is above the detected player's head
               && player->getY()<character->getY()
               // The player's body is well within the x range of the detected player's body
               && player->getX()+player->getW()>character->getX()+2 // The player's right is to the right of the detected player's left
               && player->getX()+2<character->getX()+character->getW()) { // The player's left is to the left of the detected player's right
                player->setIsOnPlatform(true);
            }

            // If below the detected player
            //The player's head is above the detected player's bottom
            if (player->getY()<character->getY()+character->getH() &&
               // The player's head is below the detected player's head
               player->getY() > character->getY()
               // The player's body is well within the x range of the detected player's body
               && player->getX()+player->getW()>character->getX()+2 // The player's right is to the right of the detected player's left
               && player->getX()+2<character->getX()+character->getW()) { // The player's left is to the left of the detected player's right
                player->setTimeSpentJumping(PRESSURE_JUMP_MAX);
            }

        }
    }
}

void Game::handleCollisionsWithCameraBorders(Player *player) {

    // Check for collisions with down camera borders
    if (player->getY() > camera.getY() + camera.getH() - player->getH() + DISTANCE_OUT_MAP_BEFORE_DEATH){
        printf("WASTED\n");

        //Temporarily resets the player to x=50 and y=50 being the player's spawn points.
        player->setX(50);
        player->setY(50);
    }

    /*// Check for collisions with down camera borders
        * if (player.y < camera.y){
         *
         * }
    */

    // Check for collisions with up camera borders
    if (player->getY() < camera.getY() - DISTANCE_OUT_MAP_BEFORE_DEATH){
        printf("WASTED\n");

        //Temporarily resets the player to x=50 and y=50 being the player's spawn points.
        player->setX(camera.getX());
        player->setY(camera.getY());
    }


    // Check for collisions with right camera borders
    if (player->getX() > camera.getX() + camera.getW() - player->getW()) {

        // Divide the velocity of the player
        player->setMoveX(player->getMoveX() / 5);

        camera.setX(camera.getX() + player->getMoveX());

        // Check if others players touch the left camera borders
        for (Player &character : characters){
            if(character.getX() < camera.getX()){
                    character.setMoveX(player->getMoveX());
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
}


void Game::handleCollisions() {
    // Check collisions for all players
    for (Player &character : characters) {
        character.setCanMove(true);

        // Check obstacles collisions only if the player has moved
        if (character.getMoveY() != 0 || character.getCurrentDirection() != 0) {
            character.setIsOnPlatform(false);
            handleCollisionsWithObstacles(&character);
        }

        handleCollisionsWithOtherPlayers(&character); // Check collisions with other players
        handleCollisionsWithDeathZone(&character); // Check collisions with death zones
        handleCollisionsWithSaveZone(&character); // Check collisions with save zones
        handleCollisionsWithPlatforms(&character); // Check collisions with platforms
        handleCollisionsWithCameraBorders(&character); // Check collisions with camera borders
    }
}


/** HANDLE COLLISIONS REVERSED MAVITY **/
void Game::handleCollisionsWithObstaclesReverseMavity(Player *player) {
    // Check collisions with each obstacle
    for (const Polygon &obstacle: level.getZones(zoneType::COLLISION)) {
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
    // Not implemented yet
}

void Game::handleCollisionsWithCameraBordersReversedMavity(Player *player) {
    // Not implemented yet
}

void Game::handleCollisionsReversedMavity() {
    // Check collisions for all players
    for (Player &character : characters) {
        character.setCanMove(true);

        // Check obstacles collisions only if the player has moved
        if (character.getMoveY() != 0 || character.getCurrentDirection() != 0) {
            character.setIsOnPlatform(false);
            handleCollisionsWithObstaclesReverseMavity(&character);

        handleCollisionsWithOtherPlayersReversedMavity(&character); // Check collisions with other players
        handleCollisionsWithPlatformsReversedMavity(&character); // Check collisions with platforms
        handleCollisionsWithCameraBordersReversedMavity(&character); // Check collisions with camera borders
        }
    }
}

void Game::render() {
    Point cam = {camera.getX(), camera.getY()};

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render textures
    if (render_textures) {
        // Draw the characters
        for (Player &character: characters) {
            character.render(renderer, cam);
        }

        level.renderPolygonsDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }
    // Render collisions box
    else {
        // Draw the characters
        for (Player const &character: characters) {
            character.renderDebug(renderer, cam);
        }

        level.renderPolygonsDebug(renderer, cam); // Draw the obstacles
        level.renderPlatformsDebug(renderer, cam); // Draw the platforms
    }

    // DEBUG DRAWING OF APPLICATION CONSOLE :

    // Draw the camera point if enabled
    if (render_camera_point) camera.renderCameraPoint(renderer, getAveragePlayersPositions());

    // Draw the camera area if enabled
    if (render_camera_area) camera.renderCameraArea(renderer);

    // Draw the player's colliders if enabled
    if (render_player_colliders) {
        Player initialPlayer = *findPlayerById(-1);
        initialPlayer.renderColliders(renderer, cam);
    }

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
        handleEvents();
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
    characters.clear();
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
