#include "SquarePrimitive.h"

SquarePrimitive::SquarePrimitive() {    
    int facesPerSquare = 6;
    int verticesPerFace = 4;
    int indicesPerFace = 6;

    int verticesPerSquare = verticesPerFace * facesPerSquare;
    int indicesPerSquare = indicesPerFace * facesPerSquare;

    vertexCount = verticesPerSquare;
    vertexData = new Vertex[verticesPerSquare];
    indexCount = indicesPerSquare;
    indexData = new uint16_t[indexCount];

    // positive x face     // position      // texture uv   // normal
    vertexData[0] =     {  1, -1, -1,       0, 0,           1,  0,  0 };
    vertexData[1] =     {  1, -1,  1,       1, 0,           1,  0,  0 };
    vertexData[2] =     {  1,  1,  1,       1, 1,           1,  0,  0 };
    vertexData[3] =     {  1,  1, -1,       0, 1,           1,  0,  0 };
    // -ve
    vertexData[4] =     { -1, -1, -1,       0, 0,          -1,  0,  0 };
    vertexData[5] =     { -1,  1, -1,       1, 0,          -1,  0,  0 };
    vertexData[6] =     { -1,  1,  1,       1, 0,          -1,  0,  0 };
    vertexData[7] =     { -1, -1,  1,       0, 1,          -1,  0,  0 };

    // positive y face
    vertexData[8] =     { -1,  1, -1,       0, 0,           0,  1,  0 };
    vertexData[9] =     {  1,  1, -1,       1, 0,           0,  1,  0 };
    vertexData[10] =    {  1,  1,  1,       1, 1,           0,  1,  0 };
    vertexData[11] =    { -1,  1,  1,       0, 1,           0,  1,  0 };
    // -ve
    vertexData[12] =    { -1, -1, -1,       0, 0,           0, -1,  0 };
    vertexData[13] =    { -1, -1,  1,       1, 0,           0, -1,  0 };
    vertexData[14] =    {  1, -1,  1,       1, 1,           0, -1,  0 };
    vertexData[15] =    {  1, -1, -1,       0, 1,           0, -1,  0 };

    // positive z face
    vertexData[16] =    { -1, -1,  1,       0, 0,           0,  0,  1 };
    vertexData[17] =    { -1,  1,  1,       1, 0,           0,  0,  1 };
    vertexData[18] =    {  1,  1,  1,       1, 1,           0,  0,  1 };
    vertexData[19] =    {  1, -1,  1,       0, 1,           0,  0,  1 };
    // -ve
    vertexData[20] =    { -1, -1, -1,       0, 0,           0,  0, -1 };
    vertexData[21] =    {  1, -1, -1,       1, 0,           0,  0, -1 };
    vertexData[22] =    {  1,  1, -1,       1, 1,           0,  0, -1 };
    vertexData[23] =    { -1,  1, -1,       0, 1,           0,  0, -1 };

    for (int i = 0; i < facesPerSquare; ++i) {
        int firstVertex = i * verticesPerFace;
        int firstIndex = i * indicesPerFace;
        indexData[firstIndex] = firstVertex;
        indexData[firstIndex + 1] = firstVertex + 1;
        indexData[firstIndex + 2] = firstVertex + 2;
        indexData[firstIndex + 3] = firstVertex + 2;
        indexData[firstIndex + 4] = firstVertex + 3;
        indexData[firstIndex + 5] = firstVertex;
    }
}

SquarePrimitive::~SquarePrimitive() {
    delete vertexData;
    delete indexData;
}