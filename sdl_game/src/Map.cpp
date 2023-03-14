#include "Map.h"
#include "TextureManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>

//0 w 1 g 2 d


Map::Map()
{
    //An order of pushing is important.
    textures.push_back(TextureManager::LoadTexture("assets/water.png"));
    textures.push_back(TextureManager::LoadTexture("assets/grass.png"));
    textures.push_back(TextureManager::LoadTexture("assets/dirt.png"));

    loadFile("assets/lvl1.txt");

    src.x = 0;
    src.y = 0;
    src.w = 32;
    src.h = 32;
    dst.x = 0;
    dst.y = 0;
    dst.w = 32;
    dst.h = 32;
}

Map::~Map() {
    for (auto& ptr: textures) {
        SDL_DestroyTexture(ptr);
    }
}

void Map::loadMap(const table& map_) {
    rows = map_.size();
    columns = map_[0].size();
    map = map_;
}

void Map::loadFile(const std::string& path) {
    std::ifstream is(path);
    std::string line;
    while (std::getline(is, line)) {
        std::istringstream ss(line);
        int tmp;
        std::vector<int> currentRow;
        while (ss >> tmp) {
            currentRow.push_back(tmp);
        }
        map.push_back(currentRow);
        currentRow.resize(0);
    }
    rows = map.size();
    columns = map[0].size();
}

void Map::drawMap() {
    for (int row = 0; row < rows; ++row) {
        dst.y = row * 32;
        for (int column = 0; column < columns; ++column) {
            dst.x = column * 32;
            int code = map[row][column];
            TextureManager::Draw(textures[code], src, dst);
        }
    }
}

