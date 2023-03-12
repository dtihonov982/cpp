#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "Game.h"


class TextureManager
{
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void Draw(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest);
};

#endif // TEXTUREMANAGER_H
