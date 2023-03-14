#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename) {
    SDL_Surface* tmp = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmp);
    SDL_FreeSurface(tmp);

    return tex;
}

void TextureManager::Draw(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest) {
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}
