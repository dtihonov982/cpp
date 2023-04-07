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
	TransformComponent* transform;
	SpriteComponent* sprite;
	
	SDL_Rect tileRect;
	int id;
	
	TileComponent() = default;
	TileComponent(int x, int y, int w, int h, int id_) {
		tileRect = {x, y, w, h};
		id = id_;
	}
	
	void init() {
		transform = &entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
		if (auto it = tileSprites.find(id); it != tileSprites.end()) {
			sprite = &entity->addComponent<SpriteComponent>(it->second.c_str());
		}
	}
};
	




#endif //TILECOMPONENT_H
