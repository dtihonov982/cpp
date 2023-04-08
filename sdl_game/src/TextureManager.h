#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "Game.h"


class TextureManager
{
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void Draw(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif // TEXTUREMANAGER_H
