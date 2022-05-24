#include "PosColorVertex.h"

class SquarePrimitive {
public:
    PosColorVertex* vertexData; // vertex buffer
    size_t vertexCount;
    uint16_t* indexData;  // index buffer
    size_t indexCount; 
    SquarePrimitive();
    ~SquarePrimitive();
};
