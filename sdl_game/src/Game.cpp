#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "future"

//Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
Entity& player = manager.addEntity();
Entity& wall = manager.addEntity();

enum GroupLabels: std::size_t {
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

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
    getComponentTypeID<Collider>();
    player.addComponent<TransformComponent>(0, 0, 32, 32, 4);
    player.addComponent<SpriteComponent>("assets/player_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>("player");
    player.addGroup(groupPlayers);
    
	Map::loadMap("assets/map.map");


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
    		//std::async([a = playerCollider.tag, b = coll->tag] { std::cout << a << " hit " << b << std::endl; } );
    	}
    }
}

std::vector<Entity*>& tiles = manager.getGroup(groupMap);
std::vector<Entity*>& players = manager.getGroup(groupPlayers);
std::vector<Entity*>& enemies = manager.getGroup(groupEnemies);

void Game::render() {
    SDL_RenderClear(renderer);
    
    auto drawEntity = [] (Entity* e) { e->draw(); };
    std::for_each(tiles.begin(), tiles.end(), drawEntity);
    std::for_each(players.begin(), players.end(), drawEntity);
    std::for_each(enemies.begin(), enemies.end(), drawEntity); 
    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

void Game::addTile(int srcX, int srcY, int dstX, int dstY) {
	Entity& tile = manager.addEntity();
	tile.addComponent<TileComponent>(srcX, srcY, dstX, dstY, "assets/terrain_ss.png");
	tile.addGroup(groupMap);
}

