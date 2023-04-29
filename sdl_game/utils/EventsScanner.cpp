#include <SDL2/SDL.h>
#include <unordered_set>
#include <iostream>

int main (int argc, char ** args) {
    SDL_Init(SDL_INIT_EVERYTHING); 
    std::cout << "Subsystems initialized..." << std::endl;
    SDL_Window* window = SDL_CreateWindow("", 0, 0, 800, 640, 0);
    SDL_Event event;

    size_t counter = 0;
    bool isRunning = true;
    while (isRunning) {
        size_t before = counter;
        while (SDL_PollEvent(&event)) {
            ++counter;
            std::cout << counter << ": event.type = 0x" << std::hex << event.type << std::dec << std::endl;
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            }
        }
        std::cout << "--- Handled " << counter - before << " events ---\n";
        SDL_Delay(1000);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};
