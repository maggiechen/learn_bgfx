#include "GeometryLoader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

const std::unordered_map<std::string, PrimitiveType> GeometryLoader::s_primitiveTypes = {
    {"square", PrimitiveType::Square},
    {"plane", PrimitiveType::Plane},
};

const std::unordered_map<std::string, LightType> GeometryLoader::s_lightTypes = {
    {"point", LightType::Point}
};
float degreesToRadians(float degrees) {
    return degrees * M_PI / (float)180;
}

void convertRotationFromDegreesToRadians(lb::Transform transform) {
    transform.rotation.x = degreesToRadians(transform.rotation.x);
    transform.rotation.y = degreesToRadians(transform.rotation.y);
    transform.rotation.z = degreesToRadians(transform.rotation.z);
}

std::ostream& operator<<(std::ostream& os, const lb::Transform& t) {
    os << "pos: " << t.position.x << ", " << t.position.y << ", " << t.position.z << std::endl;
    os << "rot: " << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << std::endl;
    return os;
}

void GeometryLoader::loadConfigFile(const char* configFile,
                                    ConfigLoadResult& configLoadResult) {
    std::ifstream file;
    file.open(configFile);
    if (!file.is_open()) {
        LOG("Couldn't open configuration file");
        return;
    }
    json j;
    file >> j;
    if (j.find("cameraSpeed") != j.end()) {
        configLoadResult.cameraSpeed = j["cameraSpeed"];
        configLoadResult.hasCameraSpeed = true;
    }

    if (j.find("mouseSensitivity") != j.end()) {
        configLoadResult.mouseSensitivity = j["mouseSensitivity"];
        configLoadResult.hasMouseSensitivity = true;
    }

    if (j.find("zoomSensitivity") != j.end()) {
        configLoadResult.zoomSensitivity = j["zoomSensitivity"];
        configLoadResult.hasZoomSensitivity = true;
    }

    if (j.find("shapes") == j.end()) {
        LOG("You have no shapes defined in the config file");
        return;
    }

    json shapes = j["shapes"];
    LOG(shapes.size() << " shape(s) loaded.");
    for (json shape : shapes) {
        auto primitive = shape["primitive"].get<std::string>();
        auto it = s_primitiveTypes.find(primitive);
        if (it == s_primitiveTypes.end()) {
            continue;;
        }
        switch (it->second) {
        case PrimitiveType::Square:
        {
            auto details = shape["details"].get<lb::Square>();
            // convert degrees to radians for rotation
            convertRotationFromDegreesToRadians(details.transform);
            configLoadResult.squares.push_back(details);
        }
        break;
        case PrimitiveType::Plane:
        {
            auto details = shape["details"].get<lb::Plane>();
            convertRotationFromDegreesToRadians(details.transform);
            configLoadResult.planes.push_back(details);
        }
        break;
        default:
            LOG("Unknown shape: " << shape);
        }
    }
    LOG("Squares loaded:" << configLoadResult.squares.size());
    LOG("Planes loaded:" << configLoadResult.planes.size());

    json lights = j["lights"];
    for (json light : lights) {
        auto lightType = light["type"].get<std::string>();
        auto it = s_lightTypes.find(lightType);
        if (it == s_lightTypes.end()) {
            continue;
        }
        switch (it->second) {
            case LightType::Point:
            {
                auto details = light["details"].get<lb::PointLight>();
                configLoadResult.pointLights.push_back(details);
            }
            break;
            default:
            LOG("Unknown light: " << light);
        }
    }
    LOG(configLoadResult.pointLights.size() << " point light(s) loaded.");
}
