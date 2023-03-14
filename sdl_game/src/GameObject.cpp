#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y): xpos(x), ypos(y)
{
    texture = TextureManager::LoadTexture(texturesheet);
}



void GameObject::update() {
    xpos++;
    ypos++;

    srcR.h = 87;
    srcR.w = 68;
    srcR.x = 0;
    srcR.y = 0;

    destR.h = srcR.h;
    destR.w = srcR.w;
    destR.x = xpos;
    destR.y = ypos;
}
void GameObject::render() {
    SDL_RenderCopy(Game::renderer, texture, &srcR, &destR);
}
