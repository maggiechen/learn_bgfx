#include "PosColorVertex.h"
bgfx::VertexLayout PosColorVertex::ms_decl; // initialize the static declaration

void PosColorVertex::init() {
    ms_decl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float) // 3 floats representing position
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true) // 4 uint8s representing color
        .end();
}