#include "tutorial.h"

int Tutorial::RunTutorial() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL error: " << SDL_GetError() << std::endl;
        return -1;
    } else {
        m_window = SDL_CreateWindow("BGFX Tutorial",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            kWidth, kHeight,
            SDL_WINDOW_SHOWN);
        if (m_window == nullptr) {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    // LOOP
    bool quit = false;
    SDL_Event currentEvent;
    while (!quit) {
        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}