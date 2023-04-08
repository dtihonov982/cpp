#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent: public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	float speed = 3.0f;
	int height = 32;
	int width = 32;
	int scale = 3;

	//todo: is really need to call default ctor of position&
    TransformComponent(): position() {
    }
    TransformComponent(int x, int y): position(x,y) {
    }
    TransformComponent(int x, int y, int w, int h, int sc): position(x,y), width(w), height(h), scale(sc) {
    }
    
    void init() override {
    	velocity.x = 0.0f;
    	velocity.y = 0.0f;
    }

    void update() override {
		position += velocity * speed;
    }

	
};

#endif // TRANSFORMCOMPONENT_H
