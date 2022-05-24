#include "PosColorVertex.h"
bgfx::VertexLayout PosColorVertex::ms_decl; // initialize the static declaration

void PosColorVertex::init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float) // 3 floats representing position
        .end();
}

std::ostream& operator<<(std::ostream& os, const PosColorVertex& v) {
    os << v.m_x << " " << v.m_y << " " << v.m_z;
    return os;
}
