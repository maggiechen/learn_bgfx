#include "Vertex.h"

class SquarePrimitive {
public:
    Vertex* vertexData; // vertex buffer
    size_t vertexCount;
    uint16_t* indexData;  // index buffer
    size_t indexCount; 
    SquarePrimitive();
    ~SquarePrimitive();
};
