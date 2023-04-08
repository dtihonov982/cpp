#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>

class Collider;

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {return isRunning;}
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<Collider*> colliders;
    static void addTile(int srcX, int srcY, int dstX, int dstY);
private:
    int cnt = 0;
    bool isRunning = false;
    SDL_Window* window;
};

#endif // GAME_H
