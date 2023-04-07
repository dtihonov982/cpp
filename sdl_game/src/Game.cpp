#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
Entity& player = manager.addEntity();
Entity& wall = manager.addEntity();

std::vector<Collider*> Game::colliders;

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
    
    player.addComponent<TransformComponent>();
    player.addComponent<SpriteComponent>("assets/player.png");
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>("player");
    
    wall.addComponent<TransformComponent>(300.0f, 300.0f, 20, 200, 1);
    wall.addComponent<SpriteComponent>("assets/dirt.png");
    wall.addComponent<Collider>("wall");
    
	Map::loadMap("assets/lvl1.txt");


}

void Game::handleEvents() {
    
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
    
    Collider& playerCollider = player.getComponent<Collider>();

    for (auto coll: colliders) {
    	if (coll != &playerCollider && Collision::AABB(*coll, playerCollider)) {
    		std::cout << playerCollider.tag << " hit " << coll->tag << std::endl;
    	}
    }
}
void Game::render() {
    SDL_RenderClear(renderer);
    manager.draw();
    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

void Game::addTile(int id, int x, int y) {
	Entity& tile = manager.addEntity();
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	
}

