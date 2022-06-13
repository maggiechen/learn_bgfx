#include "Vertex.h"
bgfx::VertexLayout Vertex::ms_decl; // initialize the static declaration

void Vertex::init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float) // 3 floats representing position
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float) // 2 floats representing uv coords
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float) // 3 floats representing normal
        .end();
}

std::ostream& operator<<(std::ostream& os, const Vertex& v) {
    os << v.x << " " << v.y << " " << v.z << " " << v.u << " " << v.v;
    return os;
}
