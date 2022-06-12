#include "PosColorVertex.h"

class PlanePrimitive {
public:
    PosColorVertex* vertexData;
    size_t vertexCount;
    uint16_t* indexData;
    size_t indexCount;
    PlanePrimitive();
    ~PlanePrimitive();
};
