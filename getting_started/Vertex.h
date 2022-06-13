#pragma once

#include <bgfx/bgfx.h>
#include <iostream>
struct Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;

    float normal_x;
    float normal_y;
    float normal_z;
    // declare variable representing the vertex layout
    // the tutorial used VertexDecl, but that was renamed to VertexLayout
    static bgfx::VertexLayout ms_decl;

    static void init();
};

std::ostream& operator<<(std::ostream& os, const Vertex& v); // for debugging purposes, for std::cout
