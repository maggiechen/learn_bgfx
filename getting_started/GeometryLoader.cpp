#include "GeometryLoader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
const std::unordered_map<std::string, PrimitiveType> GeometryLoader::s_primitiveTypes = { {"square", PrimitiveType::Square} };

void GeometryLoader::loadConfigFile(const char* configFile, std::vector<lb::Square>& squares) {
    std::ifstream file;
    file.open(configFile);
    if (!file.is_open()) {
        LOG("Couldn't open configuration file");
        return;
    }
    json j;
    file >> j;
    if (j.find("shapes") == j.end()) {
        LOG("You have no shapes defined in the config file");
        return;
    }

    json shapes = j["shapes"];
    LOG(shapes.size() << " shape(s) loaded.");

    for (json shape : shapes) {
        auto primitive = shape["primitive"].get<std::string>();
        switch (s_primitiveTypes.find(primitive)->second) {
        case PrimitiveType::Square:
        {
            auto details = shape["details"].get<lb::Square>();
            squares.push_back(details);
        }
        break;
        default:
            LOG("Unknown shape: " << shape);
        }
    }

    LOG("Squares loaded:" << squares.size());

    if (squares.size() <= 0) {
        return;
    }
}
