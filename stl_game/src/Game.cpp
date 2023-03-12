#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
Entity& player = manager.addEntity();

Game::Game(){}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems initialized..." << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }

    map = new Map();
    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("assets/player.png");

}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    }
}
void Game::update() {
    manager.refresh();
    manager.update();
}
void Game::render() {
    SDL_RenderClear(renderer);
    map->drawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

