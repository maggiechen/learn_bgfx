#pragma once
#include "JsonDefinitions.h"
#include "LightType.h"
#include "PrimitiveType.h"
#include <vector>
struct ConfigLoadResult {
    std::vector<lb::Square> squares;
    std::vector<lb::Plane> planes;
    std::vector<lb::PointLight> pointLights;
    float cameraSpeed = 0.0f;
    bool hasCameraSpeed = false;
    float mouseSensitivity = 0.0f;
    bool hasMouseSensitivity = false;
    float zoomSensitivity = 0.0f;
    bool hasZoomSensitivity = false;
};
