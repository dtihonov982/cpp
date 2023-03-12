#ifndef MAP_H
#define MAP_H
#include "Game.h"
#include <vector>
using table = std::vector<std::vector<int>>;

class Map
{
public:
    Map();
    ~Map();
    void loadMap(const std::vector<std::vector<int>>& map_);
    void drawMap();

private:
    SDL_Rect src, dst;
    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    int columns = 25;
    int rows = 20;
    table map;
};

#endif // MAP_H
