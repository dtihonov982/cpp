#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Game.h"

class GameObject
{
public:
    GameObject(const char* texturesheet, int x, int y);
    ~GameObject();
    void update();
    void render();
private:
    int xpos;
    int ypos;


    SDL_Texture* texture;
    SDL_Rect srcR, destR;
};

#endif // GAMEOBJECT_H
