#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class CameraController: public Component {
public:
	TransformComponent* transform;
	
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}
	
    //TODO: add mouse handling
	void update() override {
        Vector2D velocity{0, 0};
        if (Game::pressedKeys.count(SDLK_UP) == 1) {
            velocity.y += -1;
        }
        if (Game::pressedKeys.count(SDLK_DOWN) == 1) {
            velocity.y += 1;
        }
        if (Game::pressedKeys.count(SDLK_LEFT) == 1) {
            velocity.x += -1;
        }
        if (Game::pressedKeys.count(SDLK_RIGHT) == 1) {
            velocity.x += 1;
        }
        velocity.normalize();
        transform->velocity = velocity;
	}		
};

#endif // CAMERACONTROLLER_H
