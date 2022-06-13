#pragma once

#include "JsonDefinitions.h"
#include "Vertex.h"
#include "PrimitiveType.h"
#include "TimerTicker.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>   // this doesn't like being included in cpp files. Complains about some BX_CONFIG_DEBUG macro not being defined
#include <debugdraw/debugdraw.h> // putting this in cpp file seems to give me "expected unqualified-id before numeric constant" a lot. Not sure why

#include "CameraNavigation.h"
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
    static bool s_quit;
    static TimerTicker s_timer;
    static constexpr const bx::Vec3 s_origin = { 0.0f, 0.0f, 0.0f };
    static constexpr int kWidth = 1920;
    static constexpr int kHeight = 1080;

    bgfx::UniformHandle u_color;
    bgfx::UniformHandle u_pointLightColor;
    bgfx::UniformHandle u_pointLightPos;
    bgfx::UniformHandle u_modelMatrix;

    SDL_Window* m_window = NULL;
    bgfx::VertexBufferHandle m_squareVbh;
    bgfx::IndexBufferHandle m_squareIbh;
    bgfx::VertexBufferHandle m_planeVbh;
    bgfx::IndexBufferHandle m_planeIbh;
    bgfx::ProgramHandle m_program;
    bgfx::ShaderHandle m_vsh;
    bgfx::ShaderHandle m_fsh;

    static void OnQuitInput(SDL_Event quitEvent);
    static void Quit();
    bgfx::ShaderHandle loadShader(const char* name);
    void loadConfigFile(const char* configFile, std::vector<lb::Square>& squares);
    void SetColorUniform(bgfx::UniformHandle& uniformHandle, const char* colorString);
    void SubmitMesh(lb::Transform transform, const char* colorString, bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh);
};

