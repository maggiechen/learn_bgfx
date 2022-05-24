#include "PosColorVertex.h"

class SquareGeo {
public:
    PosColorVertex* vertexData; // vertex buffer
    size_t vertexCount;
    uint16_t* indexData;  // index buffer
    size_t indexCount; 
    SquareGeo();
    ~SquareGeo();
};
