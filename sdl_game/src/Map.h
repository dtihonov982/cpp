#ifndef MAP_H
#define MAP_H
#include "Game.h"
#include <string>

class Map
{
public:
    Map();
    ~Map();
    static void loadMap(const std::string& path);
    //void loadFile(const std::string& path);
    //void drawMap();

private:
/*
    SDL_Rect src, dst;
    std::vector<SDL_Texture*> textures;


    int columns = 0;
    int rows = 0;
    table map;
    */
};

#endif // MAP_H
