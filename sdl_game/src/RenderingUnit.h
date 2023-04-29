#ifndef RENDERINGUNIT_H
#define RENDERINGUNIT_H

#include <SDL2/SDL.h>
#include "Vector2D.h"

//Holds data about what to render on screen.
//It is allows to make all geometry stuff in one place and this is not need every component to know about how to draw themselves on screen. 
struct RenderingUnit {
    SDL_Texture* texture;
    SDL_Rect srcRect;
    int scale;
    Vector2D positionOnMap;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif
