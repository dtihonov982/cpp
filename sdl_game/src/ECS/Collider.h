#ifndef COLLIDER_H
#define COLLIDER_H

#include "ECS.h"
#include <string>
#include "Components.h"
#include <SDL2/SDL.h>

class Collider: public Component {
public:
	TransformComponent* transform;
	std::string tag;
	SDL_Rect collider;
	Collider(const std::string& tag_): tag(tag_) {}
	
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		Game::colliders.push_back(this);
	}
	
	void update() override {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }

};

#endif

