#include "GeometryLoader.h"
#include "ConfigLoadResult.h"
#include "LearnBgfx.h"
#include "PlanePrimitive.h"
#include "ShaderLoader.h"
#include "SquarePrimitive.h"
#include "Utils.h"
#include "InputManager.h"
#include <iostream>  // I/O for cout
#include <fstream>   // file stream for loading shader files

static constexpr const float error[4]{ 1, 0, 1, 1 };
static constexpr const float white[4]{ 1, 1, 1, 1 };
static constexpr const float red[4]{ 1, 0, 0, 1 };
static float pointLightPos[4] {0, 0, 0, 1};
bool LearnBgfx::s_quit = false;
// CameraNavigation LearnBgfx::s_cameraNavigation{};
// TODO: This doesn't raise any errors even if I don't free this memory. Need to get valgrind or something to verify
// this isn't leaking
LearnBgfx::~LearnBgfx() {
    bgfx::destroy(m_squareVbh);
    bgfx::destroy(m_squareIbh);
    bgfx::destroy(m_planeVbh);
    bgfx::destroy(m_planeIbh);
    bgfx::destroy(m_vsh);
    bgfx::destroy(m_fsh);
    bgfx::destroy(u_color);
}

void LearnBgfx::OnQuitInput(SDL_Event quitEvent) {
    Quit();
}

void LearnBgfx::Quit() {
    s_quit = true;
}

int LearnBgfx::Run(const char* configFile) {
    GeometryLoader loader;
    
    ConfigLoadResult configLoadResult;
    loader.loadConfigFile(configFile, configLoadResult);
    if (configLoadResult.hasCameraSpeed) {
        CameraNavigation::SetCameraSpeed(configLoadResult.cameraSpeed);
    }

    if (configLoadResult.hasMouseSensitivity) {
        CameraNavigation::SetMouseSensitivity(configLoadResult.mouseSensitivity);
    }

    if (configLoadResult.hasZoomSensitivity) {
        CameraNavigation::SetZoomSensitivity(configLoadResult.zoomSensitivity);
    }

    InputManager inputManager;

    // Map input keys/mouse/etc to actions in the application
    {
        inputManager.RegisterInputAction(SDL_QUIT, OnQuitInput);
        inputManager.RegisterKeyDownAction(SDLK_ESCAPE, Quit);
        inputManager.RegisterKeyHoldAction(SDLK_w, CameraNavigation::MoveForward);
        inputManager.RegisterKeyHoldAction(SDLK_s, CameraNavigation::MoveBackward);
        inputManager.RegisterKeyHoldAction(SDLK_a, CameraNavigation::MoveLeft);
        inputManager.RegisterKeyHoldAction(SDLK_d, CameraNavigation::MoveRight);

        inputManager.RegisterKeyDownAction(SDLK_KP_7, CameraNavigation::SetTopView);
        inputManager.RegisterKeyDownAction(SDLK_KP_1, CameraNavigation::SetFrontView);
        inputManager.RegisterKeyDownAction(SDLK_KP_3, CameraNavigation::SetRightView);
        inputManager.RegisterKeyDownAction(SDLK_LCTRL, CameraNavigation::InvertQuickView);
        inputManager.RegisterKeyUpAction(SDLK_LCTRL, CameraNavigation::DoNotInvertQuickView);

        inputManager.RegisterMouseMove(CameraNavigation::Pan);
        inputManager.RegisterMouseScroll(CameraNavigation::Zoom);
    }

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
    bgfx::reset(kWidth, kHeight, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewRect(0, 0, 0, uint16_t(kWidth), uint16_t(kHeight));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::touch(0); // apparently this creates an empty primitive.
    
    TimerTicker::Tick(); // do this once to make sure the deltaTime is accurate
    ddInit();

    u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    u_pointLightColor = bgfx::createUniform("u_pointLightColor", bgfx::UniformType::Vec4);
    u_pointLightPos = bgfx::createUniform("u_pointLightPos", bgfx::UniformType::Vec4);
    u_modelMatrix = bgfx::createUniform("u_modelMatrix", bgfx::UniformType::Mat4);
    // Have to initialize the vertex attributes!!!
    Vertex::init();

    // load shaders
    {
        ShaderLoader vLoader("v_simple.bin");
        ShaderLoader fLoader("f_simple.bin");
        m_vsh = vLoader.GetHandle();
        m_fsh = fLoader.GetHandle();
    }
    m_program = bgfx::createProgram(m_vsh, m_fsh, true); // true to destroy shaders when program is destroyed
    // fill in the vertex buffer with the position+color data and inform it of the vertex layout    

    SquarePrimitive square;
    const bgfx::Memory* squareVertexRef = bgfx::makeRef(square.vertexData, square.vertexCount * sizeof(Vertex));
    const bgfx::Memory* squareIndexRef = bgfx::makeRef(square.indexData, square.indexCount * sizeof(uint16_t));
    m_squareVbh = bgfx::createVertexBuffer(squareVertexRef, Vertex::ms_decl);
    m_squareIbh = bgfx::createIndexBuffer(squareIndexRef);

    if (!isValid(m_squareVbh)) {
        LOG("invalid vertex buffer handle");
    }

    PlanePrimitive plane;
    const bgfx::Memory* planeVertexRef = bgfx::makeRef(plane.vertexData, plane.vertexCount * sizeof(Vertex));
    const bgfx::Memory* planeIndexRef = bgfx::makeRef(plane.indexData, plane.indexCount * sizeof(uint16_t));
    m_planeVbh = bgfx::createVertexBuffer(planeVertexRef, Vertex::ms_decl);
    m_planeIbh = bgfx::createIndexBuffer(planeIndexRef);
    if (!isValid(m_planeVbh)) {
        LOG("invalid index buffer handle");
    }

    // APPLICATION LOOP
    SDL_Event currentEvent;
    while (!s_quit) {
        TimerTicker::Tick();
        while (SDL_PollEvent(&currentEvent) != 0) {
            inputManager.ProcessInputAndUpdateKeyState(currentEvent);
        }
        inputManager.ProcessFromInputState();
        // lights
        lb::PointLight& pointLight = configLoadResult.pointLights[0];
        pointLightPos[0] = pointLight.position.x;
        pointLightPos[1] = pointLight.position.y;
        pointLightPos[2] = pointLight.position.z;

        // Draw x/y/z axes
        DebugDrawEncoder dde;
	    dde.begin(0);
	    dde.drawAxis(0.0f, 0.0f, 0.0f, 12.0f);
        dde.drawOrb(pointLightPos[0], pointLightPos[1], pointLightPos[2], 0.5f);
        dde.drawGrid(Axis::Y, s_origin);
        dde.drawGrid(Axis::Z, s_origin);
        dde.end();

        // set up camera
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
        
        SetColorUniform(u_pointLightColor, pointLight.color.c_str());
        bgfx::setUniform(u_pointLightPos, pointLightPos);

        for (auto&& square : configLoadResult.squares) {
            const char* colorString = square.color.c_str();
            SubmitMesh(square.transform, colorString, m_squareVbh, m_squareIbh);
        }

        for (auto&& plane : configLoadResult.planes) {
            const char* colorString = plane.color.c_str();
            SubmitMesh(plane.transform, colorString, m_planeVbh, m_planeIbh);
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

void LearnBgfx::SetColorUniform(bgfx::UniformHandle& uniformHandle, const char* colorString) {
    if (strcmp(colorString, "red") == 0) {
        bgfx::setUniform(uniformHandle, red);
    } else if (strcmp(colorString, "white") == 0) {
        bgfx::setUniform(uniformHandle, white);
    } else {
        bgfx::setUniform(uniformHandle, error);
    }
}

void LearnBgfx::SubmitMesh(lb::Transform transform, const char* colorString, bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh) {
    // creates scaled, rotated, translated matrix
    float modelMatrix[16];
    bx::mtxSRT(modelMatrix,
        transform.scale.x,    transform.scale.y,    transform.scale.z,
        transform.rotation.x, transform.rotation.y, transform.rotation.z,
        transform.position.x, transform.position.y, transform.position.z);
    bgfx::setTransform(modelMatrix);
    bgfx::setUniform(u_modelMatrix, modelMatrix);

    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);

    SetColorUniform(u_color, colorString);

    bgfx::setState(BGFX_STATE_DEFAULT); // default renders triangles
    bgfx::submit(0, m_program); // submit primitive for rendering to view 0
}