#pragma once

#include <bgfx/bgfx.h>
#include <iostream>
struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;
    float m_u;
    float m_v;

    // declare variable representing the vertex layout
    // the tutorial used VertexDecl, but that was renamed to VertexLayout
    static bgfx::VertexLayout ms_decl;

    static void init();
};

std::ostream& operator<<(std::ostream& os, const PosColorVertex& v); // for debugging purposes, for std::cout
