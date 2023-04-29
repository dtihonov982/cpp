#include "Map.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>


Map::Map()
{

}

Map::~Map() {

}

//TODO:Now every tail loads as entity in the game.
//It will be beter to create an image here and draw another entities on them
//without redrawing all map in every game loop iteration.
void Map::loadMap(const std::string& path) {
    std::ifstream is(path);
    std::string line;
    for (int i = 0; std::getline(is, line); ++i) {
        std::istringstream ss(line);
        for (int j = 0, tmp; ss >> tmp; ++j) {       
            //tmp is a code of tail. First and second digit means coord on tailset.
            Game::addTile((tmp % 10) * 32, (tmp / 10) * 32, j * 64, i * 64);
        }
    }
}


