#include "GeometryLoader.h"
#include "LearnBgfx.h"
#include "ShaderLoader.h"
#include "SquarePrimitive.h"
#include "Utils.h"
#include "InputManager.h"
#include <iostream>  // I/O for cout
#include <fstream>   // file stream for loading shader files

static constexpr const float error[4]{ 1, 0, 1, 1 };
static constexpr const float white[4]{ 1, 1, 1, 1 };
static constexpr const float red[4]{ 1, 0, 0, 1 };

bool LearnBgfx::s_quit = false;
// CameraNavigation LearnBgfx::s_cameraNavigation{};
// TODO: This doesn't raise any errors even if I don't free this memory. Need to get valgrind or something to verify
// this isn't leaking
LearnBgfx::~LearnBgfx() {
    bgfx::destroy(u_color);
}

void LearnBgfx::OnQuitInput(SDL_Event quitEvent) {
    s_quit = true;
}

int LearnBgfx::Run(const char* configFile) {
    std::vector<lb::Square> squares;
    float cameraSpeed = 0.0f;
    float mouseSensitivity = 0.0f;
    bool hasCameraSpeed = false;
    bool hasMouseSensitivity = false;

    GeometryLoader loader;
    loader.loadConfigFile(configFile, squares, cameraSpeed, hasCameraSpeed, mouseSensitivity, hasMouseSensitivity);
    if (hasCameraSpeed) {
        CameraNavigation::SetCameraSpeed(cameraSpeed);
    }

    InputManager inputManager;
    inputManager.RegisterInputAction(SDL_QUIT, OnQuitInput);
    inputManager.RegisterKeyDownAction(SDLK_ESCAPE, OnQuitInput);
    inputManager.RegisterKeyHoldAction(SDLK_w, CameraNavigation::MoveForward);
    inputManager.RegisterKeyHoldAction(SDLK_s, CameraNavigation::MoveBackward);
    inputManager.RegisterKeyHoldAction(SDLK_a, CameraNavigation::MoveLeft);
    inputManager.RegisterKeyHoldAction(SDLK_d, CameraNavigation::MoveRight);
    inputManager.RegisterMouseMove(CameraNavigation::ProcessMouse);    
    SquarePrimitive square;

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

#ifdef LINUX
    // The X Window system is also called X11, sometimes also just "X"
    platformData.ndt = windowInfo.info.x11.display;
    platformData.nwh = (void*)(uintptr_t)windowInfo.info.x11.window;
#else
    platformData.ndt = windowInfo.info.win.hdc;
    // native window handle
    // window is an unsigned long, so truncate it to a uintptr and cast to void* since that's what the native window handle field wants
    platformData.nwh = (void*)(uintptr_t)windowInfo.info.win.window;
#endif
	
    bgfx::setPlatformData(platformData);
    bgfx::renderFrame(); // empty frame
    bgfx::Init initParams;
    initParams.type = bgfx::RendererType::OpenGL;
    bgfx::init(initParams);
    bgfx::setDebug(BGFX_DEBUG_NONE);

    u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);

    // Have to initialize the vertex attributes!!!
    PosColorVertex::init();

    // fill in the vertex buffer with the position+color data and inform it of the vertex layout
    const bgfx::Memory* vertexRef = bgfx::makeRef(square.vertexData, square.vertexCount * sizeof(PosColorVertex));
    const bgfx::Memory* indexRef = bgfx::makeRef(square.indexData, square.indexCount * sizeof(uint16_t));
    m_vbh = bgfx::createVertexBuffer(vertexRef, PosColorVertex::ms_decl);
    m_ibh = bgfx::createIndexBuffer(indexRef);

    if (!isValid(m_vbh)) {
        LOG("invalid vertex buffer handle");
    }

    // load shaders
    bgfx::ShaderHandle vsh, fsh;
    {
        ShaderLoader vLoader("v_simple.bin");
        ShaderLoader fLoader("f_simple.bin");
        vsh = vLoader.GetHandle();
        fsh = fLoader.GetHandle();
    }
    m_program = bgfx::createProgram(vsh, fsh, true); // true to destroy shaders when program is destroyed

    bgfx::reset(kWidth, kHeight, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewRect(0, 0, 0, uint16_t(kWidth), uint16_t(kHeight));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::touch(0); // apparently this creates an empty primitive.
    
    TimerTicker::Tick(); // do this once to make sure the deltaTime is accurate
    ddInit();
    // APPLICATION LOOP
    SDL_Event currentEvent;
    while (!s_quit) {
        TimerTicker::Tick();
        while (SDL_PollEvent(&currentEvent) != 0) {
            inputManager.ProcessInputAndUpdateKeyState(currentEvent);
        }
        inputManager.ProcessFromInputState();

        DebugDrawEncoder dde;
	    dde.begin(0);
	    dde.drawAxis(0.0f, 0.0f, 0.0f, 12.0f);
        dde.end();
        // set up camera
        const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
        const bx::Vec3 eye = CameraNavigation::GetEyePos();
        bx::Vec3 lookDir = CameraNavigation::GetLookDirection();

        float view[16];
        bx::mtxLookAt(view, eye, bx::add(eye, lookDir));

        float proj[16];
        // getCaps gets rendering capabilities. If homogeneuousDepth is true, then NDC is on [-1, 1]. Otherwise, [0, 1]
        bx::mtxProj(proj, 60.0f, float(kWidth) / float(kHeight), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, kWidth, kHeight);

        bgfx::touch(0);

        for (auto&& square : squares) {
            float halfSide = square.size / (float)2;
            float mtx[16];
            // creates scaled, rotated, translated matrix
            bx::mtxSRT(mtx,
                halfSide, halfSide, halfSide,
                square.transform.rotation.x, square.transform.rotation.y, square.transform.rotation.z,
                square.transform.position.x, square.transform.position.y, square.transform.position.z);
            bgfx::setTransform(mtx);
            bgfx::setVertexBuffer(0, m_vbh);
            bgfx::setIndexBuffer(m_ibh);

            const char* shapeColorString = square.color.c_str();
            if (strcmp(shapeColorString, "red") == 0) {
                bgfx::setUniform(u_color, red);
            } else if (strcmp(shapeColorString, "white") == 0) {
                bgfx::setUniform(u_color, white);
            } else {
                bgfx::setUniform(u_color, error);
            }

            bgfx::setState(BGFX_STATE_DEFAULT); // default renders triangles
            bgfx::submit(0, m_program); // submit primitive for rendering to view 0
        }
        bgfx::frame();
    }

    // clean up
    ddShutdown();
    SDL_DestroyWindow(m_window);
    bgfx::shutdown();
    SDL_Quit();
    return 0;
}
