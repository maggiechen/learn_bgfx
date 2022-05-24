#pragma once

#include "PosColorVertex.h"
#include "PrimitiveType.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>   // this doesn't like being included in cpp files. Complains about some BX_CONFIG_DEBUG macro not being defined
#ifdef LINUX
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_syswm.h>
#else
    #include <SDL.h>
    #include <SDL_syswm.h>
#endif
#include <unordered_map>
#include <string>
#include <vector>

namespace lb {
    struct Square;
}

class LearnBgfx {
public:
    int Run(const char* configFile);
    ~LearnBgfx();
private:

    static constexpr int kWidth = 640;
    static constexpr int kHeight = 480;

    bgfx::UniformHandle u_color;

    SDL_Window* m_window = NULL;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
private:
    bgfx::ShaderHandle loadShader(const char* name);
    void loadConfigFile(const char* configFile, std::vector<lb::Square>& squares);
};

