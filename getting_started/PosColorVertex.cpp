#include "PosColorVertex.h"
bgfx::VertexLayout PosColorVertex::ms_decl; // initialize the static declaration

void PosColorVertex::init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float) // 3 floats representing position
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true) // 4 uint8s representing color
        .end();
}

std::ostream& operator<<(std::ostream& os, const PosColorVertex& v) {
    os << v.m_x << " " << v.m_y << " " << v.m_z << " " << v.m_abgr;
    return os;
}
