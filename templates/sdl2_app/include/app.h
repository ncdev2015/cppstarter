#pragma once

#include <SDL.h>

class App {
public:
    App();
    ~App();

    bool init();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
};
