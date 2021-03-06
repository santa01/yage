#include "Game.h"
#include "Logger.h"
#include "configuration.h"

Game::Game():
        renderer(WIDTH, HEIGHT, "Game", FULLSCREEN) {
}

void Game::start() {
    Logger::getInstance().setThreshold(VERBOSE);
    this->renderer.run();
}
