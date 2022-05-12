#pragma once

#include <bgfx/bgfx.h>

struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;

    // color
    uint32_t m_abgr;

    // declare variable representing the vertex layout
    // the tutorial used VertexDecl, but that was renamed to VertexLayout
    static bgfx::VertexLayout ms_decl;

    static void init();
};

static PosColorVertex s_cubeVertices[] =
{   // position           color
    {  0.5f,  0.5f, 0.0f, 0xff0000ff },
    {  0.5f, -0.5f, 0.0f, 0xff0000ff },
    { -0.5f, -0.5f, 0.0f, 0xff00ff00 },
    { -0.5f,  0.5f, 0.0f, 0xff00ff00 }
};

static const uint16_t s_cubeTriList[] =
{
    0,1,3,
    1,2,3
};