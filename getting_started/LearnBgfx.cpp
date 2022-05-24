#define LOG(x) \
    std::cout << x << std::endl

#include "LearnBgfx.h"
#include <json.hpp>
#include <iostream>  // I/O for cout
#include <fstream>   // file stream for loading shader files
#include <vector>

bool useDynamic = true;

using json = nlohmann::json;
const std::unordered_map<std::string, LearnBgfx::PrimitiveType> LearnBgfx::s_primitiveTypes = { {"square", PrimitiveType::Square} };

namespace lb {
    struct Position {
        float x;
        float y;
        float z;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Position, x, y, z)

        struct Transform {
        Position position;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, position)

        struct Square {
        float size;
        std::string color;
        Transform transform;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Square, size, color, transform)
}

// TODO: This doesn't raise any errors even if I don't free this memory. Need to get valgrind or something to verify
// this isn't leaking
LearnBgfx::~LearnBgfx() {
    delete m_vertexData;
    delete m_indexData;
}

int LearnBgfx::Run(const char* configFile) {
    loadConfigFile(configFile);

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

    // Have to initialize the vertex attributes!!!
    PosColorVertex::init();

    // fill in the vertex buffer with the position+color data and inform it of the vertex layout
    const bgfx::Memory* vertexRef = bgfx::makeRef(m_vertexData, m_vertexCount * sizeof(PosColorVertex));
    const bgfx::Memory* indexRef = bgfx::makeRef(m_indexData, m_indexCount * sizeof(uint16_t));
    m_vbh = bgfx::createVertexBuffer(vertexRef, PosColorVertex::ms_decl);
    m_ibh = bgfx::createIndexBuffer(indexRef);

    if (!isValid(m_vbh)) {
        LOG("invalid vertex buffer handle");
    }
    // debugging
    // for (int i = 0; i < m_vertexCount; ++i) {
    //     LOG(m_vertexData[i]);
    // }
    // for (int i = 0; i < m_indexCount; ++i) {
    //     LOG(m_indexData[i]);
    // }

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
            switch (currentEvent.type) {
            case SDL_QUIT:
                quit = true;
            case SDL_KEYDOWN:
                if (currentEvent.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
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

        bgfx::setState(BGFX_STATE_DEFAULT); // default renders triangles
        bgfx::submit(0, m_program); // submit primitive for rendering to view 0

        bgfx::frame();
    }

    // clean up
    SDL_DestroyWindow(m_window);
    bgfx::shutdown();
    SDL_Quit();
    return 0;
}

bgfx::ShaderHandle LearnBgfx::loadShader(const char* name) {
    char* shaderData = new char[2048];
    std::ifstream file;
    size_t fileSize = 0;
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
    delete[] shaderData;
    return handle;
}

void LearnBgfx::loadConfigFile(const char* configFile) {
    std::ifstream file;
    file.open(configFile);
    if (!file.is_open()) {
        LOG("Couldn't open configuration file");
        return;
    }
    json j;
    file >> j;
    if (j.find("shapes") == j.end()) {
        LOG("You have no shapes defined in the config file");
        return;
    }

    json shapes = j["shapes"];
    LOG(shapes.size() << " shape(s) loaded.");
    std::vector<lb::Square> squares;

    for (json shape : shapes) {
        auto primitive = shape["primitive"].get<std::string>();
        switch (LearnBgfx::s_primitiveTypes.find(primitive)->second) {
        case PrimitiveType::Square:
        {
            auto details = shape["details"].get<lb::Square>();
            squares.push_back(std::move(details));
        }
        break;
        default:
            LOG("Unknown shape: " << shape);
        }
    }

    LOG("Squares loaded:" << squares.size());
    int facesPerSquare = 6;
    int verticesPerFace = 4;
    int indicesPerFace = 6;

    int verticesPerSquare = verticesPerFace * facesPerSquare;
    int indicesPerSquare = indicesPerFace * facesPerSquare;

    m_vertexCount = squares.size() * verticesPerSquare;
    m_vertexData = new PosColorVertex[m_vertexCount];
    m_indexCount = squares.size() * indicesPerSquare;
    m_indexData = new uint16_t[m_indexCount];

    LOG(m_vertexCount << " " << m_indexCount);

    for (int square_i = 0; square_i < squares.size(); ++square_i) {
        // sorry, I hate this name too, but this is the index into the "index buffer"
        // that determines the order that vertices create triangles
        int index_i = square_i * indicesPerSquare;
        int vertex_i = square_i * verticesPerSquare;
        lb::Square& square = squares[square_i];
        lb::Position& p = square.transform.position;

        float sideHalf = squares[square_i].size / 2;
        uint color = 0xffffffff;
        if (strcmp(squares[square_i].color.c_str(), "red") == 0) {
            color = 0xffffffff;
        }
        LOG("Color " << color);
        // positive x face
        m_vertexData[vertex_i] = { sideHalf, -sideHalf, -sideHalf, color };
        m_vertexData[vertex_i + 1] = { sideHalf, -sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 2] = { sideHalf, sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 3] = { sideHalf, sideHalf, -sideHalf, color };
        // -ve
        m_vertexData[vertex_i + 4] = { -sideHalf, -sideHalf, -sideHalf, color };
        m_vertexData[vertex_i + 5] = { -sideHalf, -sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 6] = { -sideHalf, sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 7] = { -sideHalf, sideHalf, -sideHalf, color };

        // positive y face
        m_vertexData[vertex_i + 8] = { -sideHalf, sideHalf,  -sideHalf, color };
        m_vertexData[vertex_i + 9] = { -sideHalf, sideHalf,  sideHalf, color };
        m_vertexData[vertex_i + 10] = { sideHalf, sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 11] = { sideHalf, sideHalf, -sideHalf, color };
        // -ve
        m_vertexData[vertex_i + 12] = { -sideHalf, -sideHalf,  -sideHalf, color };
        m_vertexData[vertex_i + 13] = { -sideHalf, -sideHalf,  sideHalf, color };
        m_vertexData[vertex_i + 14] = { sideHalf, -sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 15] = { sideHalf, -sideHalf, -sideHalf, color };

        // positive z face
        m_vertexData[vertex_i + 16] = { -sideHalf,  -sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 17] = { -sideHalf,  sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 18] = { sideHalf, sideHalf, sideHalf, color };
        m_vertexData[vertex_i + 19] = { sideHalf, -sideHalf, sideHalf, color };
        // -ve
        m_vertexData[vertex_i + 20] = { -sideHalf,  -sideHalf, -sideHalf, color };
        m_vertexData[vertex_i + 21] = { -sideHalf,  sideHalf, -sideHalf, color };
        m_vertexData[vertex_i + 22] = { sideHalf, sideHalf, -sideHalf, color };
        m_vertexData[vertex_i + 23] = { sideHalf, -sideHalf, -sideHalf, color };
        LOG("Finished populating vertices");
        for (int i = 0; i < 6; ++i) {
            int firstVertex = vertex_i + i * verticesPerFace;
            int firstIndex = index_i + i * indicesPerFace;
            m_indexData[firstIndex] = firstVertex;
            m_indexData[firstIndex + 1] = firstVertex + 1;
            m_indexData[firstIndex + 2] = firstVertex + 2;
            m_indexData[firstIndex + 3] = firstVertex + 2;
            m_indexData[firstIndex + 4] = firstVertex + 3;
            m_indexData[firstIndex + 5] = firstVertex;
        }
        LOG("Finished indices");
    }
}

