#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>

class Collider;

class Collision {
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const Collider& colA, const Collider& colB);
};

#endif
