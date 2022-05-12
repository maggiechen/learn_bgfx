#pragma once

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <iostream>

class Tutorial {
public:
    int RunTutorial();

private:
    static constexpr int kWidth = 640;
    static constexpr int kHeight = 480;
    SDL_Window* m_window = NULL;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
};
