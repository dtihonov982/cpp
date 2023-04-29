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
std::unordered_set<SDL_Keycode> Game::pressedKeys;
std::queue<RenderingUnit> Game::renderingQueue;
Manager manager;
Entity& player = manager.addEntity(GroupLabel::players);
Entity& camera = manager.addEntity();

std::vector<Collider*> Game::colliders;

Game::Game(){
}

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
    player.addComponent<TransformComponent>(0, 0, 32, 32, 4);
    player.addComponent<SpriteComponent>("assets/player_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>("player");

    camera.addComponent<TransformComponent>(0, 0, width, height, 2);
    camera.addComponent<CameraController>();
    
	Map::loadMap("assets/map.map");
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            pressedKeys.insert(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            pressedKeys.erase(event.key.keysym.sym);
            break;
        }
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

//using array
const std::vector<GroupLabel> renderingOrder = {GroupLabel::tiles, GroupLabel::players};

void Game::render() {
    SDL_RenderClear(renderer);
    
    manager.draw(renderingOrder);
    SDL_Rect dstRect;
    Vector2D cameraPosition{ camera.getComponent<TransformComponent>().position };
    while (!renderingQueue.empty()) {
        RenderingUnit unit = renderingQueue.front();
        renderingQueue.pop();

        dstRect.x = unit.positionOnMap.x - cameraPosition.x;
        dstRect.y = unit.positionOnMap.y - cameraPosition.y;
        dstRect.w = unit.srcRect.w * unit.scale;
        dstRect.h = unit.srcRect.h * unit.scale;

        TextureManager::Draw(unit.texture, unit.srcRect, dstRect, unit.flip);
    }

    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

void Game::addTile(int srcX, int srcY, int dstX, int dstY) {
	Entity& tile = manager.addEntity(GroupLabel::tiles);
	tile.addComponent<TileComponent>(srcX, srcY, dstX, dstY, "assets/terrain_ss.png");
}

