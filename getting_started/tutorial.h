#pragma once


#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>   // this doesn't like being included in cpp files. Complains about some BX_CONFIG_DEBUG macro not being defined
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

class Tutorial {
public:
    int RunTutorial();

private:
    static constexpr int kWidth = 640;
    static constexpr int kHeight = 480;
    SDL_Window* m_window = NULL;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
private:
    bgfx::ShaderHandle loadShader(const char* name);

};
