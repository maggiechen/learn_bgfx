#include "tutorial.h"

#include "PosColorVertex.h"

#include <iostream>  // I/O for cout
#include <fstream>   // file stream for loading shader files

int Tutorial::RunTutorial() {
    // Initialize SDL window
    {
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
    }

    // Get SDL window info
    SDL_SysWMinfo windowInfo; // initialize some window info to populate
    SDL_VERSION(&windowInfo.version); // set the SDL version on the window info
    if (!SDL_GetWindowWMInfo(m_window, &windowInfo)) { // populate window info
        std::cout << "Could not get window info" << std::endl;
        return -1;
    }

    // Tell BGFX what's up
        // now we get our linux specific mojo down
    bgfx::PlatformData platformData;
    // native display type
    platformData.ndt = windowInfo.info.x11.display; // The X Window system is also called X11, sometimes also just "X"
    // native window handle
    // window is an unsigned long, so truncate it to a uintptr and cast to void* since that's what the native window handle field wants
    platformData.nwh = (void*)(uintptr_t)windowInfo.info.x11.window;

    bgfx::setPlatformData(platformData);
    bgfx::renderFrame(); // empty frame
    bgfx::Init initParams;
    initParams.type = bgfx::RendererType::OpenGL;
    bgfx::init(initParams);

    // fill in the vertex buffer with the position+color data and inform it of the vertex layout
    m_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        PosColorVertex::ms_decl
    );
    if (!isValid(m_vbh)) {
        std::cout << "invalid vertex buffer handle" << std::endl;
    }
    // makeRef should be used on static data like the static arrays of s_cubeVertices and s_cubeTriList
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    // load shaders
    bgfx::ShaderHandle vsh = loadShader("v_simple.bin");
    bgfx::ShaderHandle fsh = loadShader("f_simple.bin");
    m_program = bgfx::createProgram(vsh, fsh, true); // true to destroy shaders when program is destroyed

    bgfx::reset(kWidth, kHeight, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewRect(0, 0, 0, uint16_t(kWidth), uint16_t(kHeight));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::touch(0); // apparently this creates an empty primitive.

    // APPLICATION LOOP
    bool quit = false;
    SDL_Event currentEvent;
    while (!quit) {
        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }
        }
        // set up camera
        {
            const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
            const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };

            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            // getCaps gets rendering capabilities. If homogeneuousDepth is true, then NDC is on [-1, 1]. Otherwise, [0, 1]
            bx::mtxProj(proj, 60.0f, float(kWidth) / float(kHeight), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(0, view, proj);
            bgfx::setViewRect(0, 0, 0, kWidth, kHeight);
        }
        bgfx::touch(0);

        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);
        // set x/y/z = 0;
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        bgfx::setTransform(mtx);
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // bgfx::setState(BGFX_STATE_DEFAULT); // default renders triangles
        // bgfx::submit(0, m_program); // submit primitive for rendering to view 0

        bgfx::frame();
    }

    // clean up
    SDL_DestroyWindow(m_window);
    bgfx::shutdown();
    SDL_Quit();
    return 0;
}

bgfx::ShaderHandle Tutorial::loadShader(const char* name) {
    char* shaderData = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(name);
    if (!file.is_open()) {
        std::cout << "Could not open shader file" << std::endl;
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

    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}
