#include "PlanePrimitive.h"

PlanePrimitive::PlanePrimitive() {
    vertexCount = 4;
    vertexData = new PosColorVertex[vertexCount];
    vertexData[0] = {-0.5, 0, -0.5, 0, 0};
    vertexData[1] = { 0.5, 0, -0.5, 1, 0};
    vertexData[2] = { 0.5, 0,  0.5, 0, 1};
    vertexData[3] = {-0.5, 0,  0.5, 1, 1};
    
    indexCount = 6;
    indexData = new uint16_t[indexCount];
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 2;
    indexData[4] = 3;
    indexData[5] = 0;
}

PlanePrimitive::~PlanePrimitive() {
    delete vertexData;
    delete indexData;
}