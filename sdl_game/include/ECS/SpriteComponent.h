#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "Components.h"
#include "SDL2/SDL.h"
#include "TextureManager.h"


class SpriteComponent: public Component
{
public:
    SpriteComponent(const char* filename) {
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
        dstR.x = transform->position.x;
        dstR.y = transform->position.y;
        dstR.w = transform->width * transform->scale;
        dstR.h = transform->height * transform->scale;
        
    }
    void draw() override {
        TextureManager::Draw(texture, srcR, dstR);
    }

private:
    TransformComponent* transform;
    SDL_Texture* texture = nullptr;
    SDL_Rect srcR, dstR;
};

#endif // SPRITECOMPONENT_H
