#include "Vertex.h"

class PlanePrimitive {
public:
    Vertex* vertexData;
    size_t vertexCount;
    uint16_t* indexData;
    size_t indexCount;
    PlanePrimitive();
    ~PlanePrimitive();
};
