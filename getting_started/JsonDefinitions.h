#pragma once
#include <json.hpp>
using json = nlohmann::json;

namespace lb {
    struct Position {
        float x;
        float y;
        float z;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Position, x, y, z);
    struct Rotation {
        float x;
        float y;
        float z;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rotation, x, y, z);
    struct Scale {
        float x;
        float y;
        float z;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Scale, x, y, z);

    struct Transform {
        Position position;
        Rotation rotation;
        Scale scale;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, position, rotation, scale);

    struct Square {
        std::string color;
        Transform transform;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Square, color, transform)
    
    struct Plane {
        std::string color;
        Transform transform;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Plane, color, transform)

    struct PointLight {
        std::string color;
        Position position;
        float intensity;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PointLight, color, position, intensity);
}
