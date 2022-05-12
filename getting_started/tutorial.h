#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

class Tutorial {
public:
    int RunTutorial();

private:
    static constexpr int kWidth = 640;
    static constexpr int kHeight = 480;
    SDL_Window* m_window = NULL;
};
