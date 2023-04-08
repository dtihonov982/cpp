#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "Components.h"
#include "SDL2/SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include <tuple>

class SpriteComponent: public Component
{
public:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	
    SpriteComponent(const char* filename) {
        setTex(filename);
    }
    SpriteComponent(const char* filename, bool isAnimated) {
    	animated = isAnimated;
    	Animation idle{0, 3, 100};
    	Animation walk{1, 8, 100};
    	animations.insert({"idle", idle});
    	animations.insert({"walk", walk});
    	
    	play("idle");
    	
        setTex(filename);
    }
    ~SpriteComponent() {
    	SDL_DestroyTexture(texture);
    }
    void setTex(const char* filename) {
        if (!texture) {
            SDL_DestroyTexture(texture);
        }
        texture = TextureManager::LoadTexture(filename);
    }
    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        srcR.x = srcR.y = 0;
        srcR.w = transform->width;
        srcR.h = transform->height;
    }
    void update() override {
    	if (animated) {
    		srcR.x = srcR.w * static_cast<int>((SDL_GetTicks() / delay) % frames);
    	}
    	srcR.y = animIndex * transform->height;
    	
        dstR.x = transform->position.x;
        dstR.y = transform->position.y;
        dstR.w = transform->width * transform->scale;
        dstR.h = transform->height * transform->scale;
        
    }
    void draw() override {
        TextureManager::Draw(texture, srcR, dstR, spriteFlip);
    }
    
    void play(const char* animName) {
    	Animation& anim = animations[animName];
    	frames = anim.frames;
    	delay = anim.speed;
    	animIndex = anim.index;
    }

private:
    TransformComponent* transform;
    SDL_Texture* texture = nullptr;
    SDL_Rect srcR, dstR;
    bool animated = false;
    int frames = 0;
    int delay = 100;
};

#endif // SPRITECOMPONENT_H
