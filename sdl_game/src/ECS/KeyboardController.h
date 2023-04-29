#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController: public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}
	
	void update() override {
        //TODO: flip
        Vector2D velocity{0, 0};
        if (Game::pressedKeys.count(SDLK_w) == 1) {
            velocity.y += -1;
        }
        if (Game::pressedKeys.count(SDLK_s) == 1) {
            velocity.y += 1;
        }
        if (Game::pressedKeys.count(SDLK_a) == 1) {
            velocity.x += -1;
        }
        if (Game::pressedKeys.count(SDLK_d) == 1) {
            velocity.x += 1;
        }
        velocity.normalize();
        transform->velocity = velocity;
	}		

};

#endif // KEYBOARDCONTROLLER_H