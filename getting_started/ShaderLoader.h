#pragma once
#include <bgfx/bgfx.h>

class ShaderLoader {
    private:
    bgfx::ShaderHandle m_handle;
    public:
    ShaderLoader(const char* name);
    bgfx::ShaderHandle& GetHandle();
    ~ShaderLoader();
};
