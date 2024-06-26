#include "../../../include/Game/GameManagers/RenderManager.h"

/**
 * @file RenderManager.cpp
 * @brief Implements the RenderManager class responsible for rendering the game.
 */


/* CONSTRUCTORS */

std::vector<TTF_Font*> RenderManager::fonts;

RenderManager::RenderManager(SDL_Renderer *renderer, Game *game) : renderer(renderer), gamePtr(game) {

    // Load the textures
    if (!Player::loadTextures(*renderer)) {
        std::cerr << "Error loading player textures" << std::endl;
        exit(1);
    }
    if (!Asteroid::loadTextures(*renderer)) {
        std::cerr << "Error loading asteroid textures" << std::endl;
        exit(1);
    }
    if(!Coin::loadTexture(*renderer)){
        std::cerr << "Error loading coin textures" << std::endl;
        exit(1);
    }
    // Load the fonts
    TTF_Font *font16 = TTF_OpenFont("assets/font/arial.ttf", 16);
    TTF_Font *font24 = TTF_OpenFont("assets/font/arial.ttf", 24);
    for (TTF_Font const* font : fonts) {
        if (font == nullptr) {
            std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
            exit(1);
        }
    }

    fonts.push_back(font16);
    fonts.push_back(font24);
}


/* ACCESSORS */

SDL_Renderer* RenderManager::getRenderer() {
    return renderer;
}

std::vector<TTF_Font *> &RenderManager::getFonts() {
    return fonts;
}

void RenderManager::setRenderTextures(bool renderTextures) {
    render_textures = renderTextures;
}

void RenderManager::setRenderCameraPoint(bool renderCameraPoint) {
    render_camera_point = renderCameraPoint;
}

void RenderManager::setRenderCameraArea(bool renderCameraArea) {
    render_camera_area = renderCameraArea;
}

void RenderManager::setRenderPlayerColliders(bool renderPlayerColliders) {
    render_player_colliders = renderPlayerColliders;
}

void RenderManager::setRenderFps(bool renderFps) {
    render_fps = renderFps;
}

void RenderManager::toggleRenderTextures() {
    render_textures = !render_textures;
}

void RenderManager::toggleRenderCameraPoint() {
    render_camera_point = !render_camera_point;
}

void RenderManager::toggleRenderCameraArea() {
    render_camera_area = !render_camera_area;
}

void RenderManager::toggleRenderPlayerColliders() {
    render_player_colliders = !render_player_colliders;
}

void RenderManager::toggleRenderFps() {
    render_fps = !render_fps;
}


/* METHODS */

void RenderManager::render() {
    Level *level = gamePtr->getLevel();
    PlayerManager &playerManager = gamePtr->getPlayerManager();

    Point camera_point = gamePtr->getCamera()->getRenderingPoint();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render textures
    if (render_textures) {
        // Draw the environment
        level->renderBackgrounds(renderer, camera_point); // Draw the background

        level->renderPolygonsDebug(renderer, camera_point); // Draw the obstacles
        level->renderItems(renderer, camera_point); // Draw the items
        level->renderLevers(renderer, camera_point); // Draw the levers

        // Draw the players
        for (Player &player : playerManager.getDeadPlayers()) player.render(renderer, camera_point);
        for (Player &player : playerManager.getNeutralPlayers()) player.render(renderer, camera_point);
        for (Player &player : playerManager.getAlivePlayers()) player.render(renderer, camera_point);

        level->renderAsteroids(renderer, camera_point); // Draw the asteroids
        level->renderPlatforms(renderer, camera_point); // Draw the platforms
        level->renderTraps(renderer, camera_point); // Draw the traps

        level->renderMiddleground(renderer, camera_point); // Draw the middleground
        level->renderForegrounds(renderer, camera_point); // Draw the foreground
    }

    // Render collision boxes
    else {
        level->renderAsteroidsDebug(renderer, camera_point); // Draw the asteroids
        level->renderPolygonsDebug(renderer, camera_point); // Draw the obstacles
        level->renderLeversDebug(renderer, camera_point); // Draw the levers
        level->renderPlatformsDebug(renderer, camera_point); // Draw the platforms
        level->renderTrapsDebug(renderer, camera_point); // Draw the traps
        level->renderItemsDebug(renderer, camera_point); // Draw the items

        // Draw the players
        for (const Player &player : playerManager.getDeadPlayers()) player.renderDebug(renderer, camera_point);
        for (const Player &player : playerManager.getNeutralPlayers()) player.renderDebug(renderer, camera_point);
        for (const Player &player : playerManager.getAlivePlayers()) player.renderDebug(renderer, camera_point);

    }

    // Render the fps counter
    if (render_fps) {
        SDL_Color color = {160, 160, 160, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fonts[0], std::to_string(gamePtr->getEffectiveFrameRate()).c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {10, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Render the camera point
    if (render_camera_point) {
        Point averagePlayersPosition = playerManager.getAveragePlayerPosition();
        gamePtr->getCamera()->renderCameraPoint(renderer, averagePlayersPosition);
    }

    // Render the camera area
    if (render_camera_area) {
        gamePtr->getCamera()->renderCameraArea(renderer);
    }

    // Render the player colliders
    if (render_player_colliders) {
        for (const Player &player : playerManager.getAlivePlayers()) player.renderColliders(renderer, camera_point);
        for (const Player &player : playerManager.getNeutralPlayers()) player.renderColliders(renderer, camera_point);
        for (const Player &player : playerManager.getDeadPlayers()) player.renderColliders(renderer, camera_point);
    }

    // If the game is paused, render the menu
    if (gamePtr->getGameState() == GameState::PAUSED) {
        // Add a transparent gray overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 180);
        SDL_Rect overlayRect = {0, 0, 1280, 720};
        SDL_RenderFillRect(renderer, &overlayRect);

        // Render the menu
        Mediator::renderMenu();
    }

    SDL_RenderPresent(renderer);
}