#include "GeometryLoader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
const std::unordered_map<std::string, PrimitiveType> GeometryLoader::s_primitiveTypes = {
    {"square", PrimitiveType::Square},
    {"plane", PrimitiveType::Plane}
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
                                    std::vector<lb::Square>& squares,
                                    std::vector<lb::Plane>& planes,
                                    float& cameraSpeed, bool& hasCameraSpeed,
                                    float& mouseSensitivity, bool& hasMouseSensitivity,
                                    float& zoomSensitivity, bool& hasZoomSensitivity) {
    std::ifstream file;
    file.open(configFile);
    if (!file.is_open()) {
        LOG("Couldn't open configuration file");
        return;
    }
    json j;
    file >> j;
    if (j.find("cameraSpeed") != j.end()) {
        cameraSpeed = j["cameraSpeed"];
        hasCameraSpeed = true;
    }

    if (j.find("mouseSensitivity") != j.end()) {
        mouseSensitivity = j["mouseSensitivity"];
        hasMouseSensitivity = true;
    }

    if (j.find("zoomSensitivity") != j.end()) {
        zoomSensitivity = j["zoomSensitivity"];
        hasZoomSensitivity = true;
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
            squares.push_back(details);
        }
        break;
        case PrimitiveType::Plane:
        {
            auto details = shape["details"].get<lb::Plane>();
            convertRotationFromDegreesToRadians(details.transform);
            planes.push_back(details);
        }
        break;
        default:
            LOG("Unknown shape: " << shape);
        }
    }

    LOG("Squares loaded:" << squares.size());
    LOG("Planes loaded:" << planes.size());
}
