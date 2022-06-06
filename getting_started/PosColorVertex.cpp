#include "PosColorVertex.h"
bgfx::VertexLayout PosColorVertex::ms_decl; // initialize the static declaration

void PosColorVertex::init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float) // 3 floats representing position
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float) // 2 floats representing uv coords
        .end();
}

std::ostream& operator<<(std::ostream& os, const PosColorVertex& v) {
    os << v.m_x << " " << v.m_y << " " << v.m_z << " " << v.m_u << " " << v.m_v;
    return os;
}
