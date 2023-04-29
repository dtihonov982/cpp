#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.h"
#include <SDL2/SDL.h>
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <unordered_map>

static std::unordered_map<int, std::string> tileSprites = {
	{0, "assets/water.png"},
	{1, "assets/grass.png"},
	{2, "assets/dirt.png"}
};

class TileComponent: public Component {
public:
	SDL_Texture* texture;
	SDL_Rect srcR, dstR;
	
	TileComponent() = default;
	TileComponent(int srcX, int srcY, int dstX, int dstY, const char* path) {
		texture = TextureManager::LoadTexture(path);
		srcR = {srcX, srcY, 32, 32};
		dstR = {dstX, dstY, 64, 64};
	}
	~TileComponent() {
		SDL_DestroyTexture(texture);
	}
	
	void draw() override {
        Game::renderingQueue.push({texture, srcR, 2, Vector2D{dstR.x, dstR.y}});
		//TextureManager::Draw(texture, srcR, dstR);
	}
	
	/*
	void init() {
		transform = &entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
		if (auto it = tileSprites.find(id); it != tileSprites.end()) {
			sprite = &entity->addComponent<SpriteComponent>(it->second.c_str());
		}
	}
	*/
};
	




#endif //TILECOMPONENT_H
