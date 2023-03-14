#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"


class SpriteComponent: public Component
{
public:
    SpriteComponent(const char* filename) {
        setTex(filename);
    }
    void setTex(const char* filename) {
        if (!texture) {
            SDL_DestroyTexture(texture);
        }
        texture = TextureManager::LoadTexture(filename);
    }
    void init() override {
        position = &entity->getComponent<PositionComponent>();
        srcR.x = srcR.y = 0;
        srcR.w = srcR.h = 87;
        dstR.w = dstR.h = 68;
    }
    void update() override {
        dstR.x = position->x();
        dstR.y = position->y();
    }
    void draw() override {
        TextureManager::Draw(texture, srcR, dstR);
    }

private:
    PositionComponent* position;
    SDL_Texture* texture = nullptr;
    SDL_Rect srcR, dstR;
};

#endif // SPRITECOMPONENT_H
