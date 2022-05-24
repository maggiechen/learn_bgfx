#include "ShaderLoader.h"
#include "Utils.h"
#include <fstream>

ShaderLoader::ShaderLoader(const char* name) {

    char* shaderData = new char[2048];
    std::ifstream file;
    size_t fileSize = 0;
    file.open(name);
    if (!file.is_open()) {
        LOG("Could not open shader file");
    } else {
        file.seekg(0, std::ios::end); // seek to the end of the file, with an offset of 0, so exactly the end
        fileSize = file.tellg(); // gets the g index I guess?
        // seek to start and read
        file.seekg(0, std::ios::beg);
        file.read(shaderData, fileSize);
        file.close();
    }
    // copy memory into a buffer. Add an extra null termination character
    const bgfx::Memory* mem = bgfx::copy(shaderData, fileSize + 1);
    mem->data[mem->size - 1] = '\0';

    m_handle = bgfx::createShader(mem);
    bgfx::setName(m_handle, name);
    delete[] shaderData;
}

ShaderLoader::~ShaderLoader() {
    bgfx::destroy(m_handle);
}

bgfx::ShaderHandle& ShaderLoader::GetHandle() {
    return m_handle;
}