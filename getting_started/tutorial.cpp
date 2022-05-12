#include "tutorial.h"
#include "PosColorVertex.h"

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
    {
        // now we get our linux specific mojo down
        bgfx::PlatformData platformData;
        // native display type
        platformData.ndt = windowInfo.info.x11.display; // The X Window system is also called X11, sometimes also just "X"
        // native window handle
        // window is an unsigned long, so truncate it to a uintptr and cast to void* since that's what the native window handle field wants
        platformData.nwh = (void*)(uintptr_t)windowInfo.info.x11.window;

        bgfx::setPlatformData(platformData);
        bgfx::renderFrame(); // empty frame
        bgfx::init();

        // fill in the vertex buffer with the position+color data and inform it of the vertex layout
        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
            PosColorVertex::ms_decl
        );
        // makeRef should be used on static data like the static arrays of s_cubeVertices and s_cubeTriList
        m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

        bgfx::reset(kWidth, kHeight, BGFX_RESET_VSYNC);
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        bgfx::setViewRect(0, 0, 0, uint16_t(kWidth), uint16_t(kHeight));
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::touch(0); // apparently this creates an empty primitive.

    }

    // APPLICATION LOOP
    bool quit = false;
    SDL_Event currentEvent;
    while (!quit) {
        bgfx::frame();

        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // clean up
    SDL_DestroyWindow(m_window);
    bgfx::shutdown();
    SDL_Quit();
    return 0;
}