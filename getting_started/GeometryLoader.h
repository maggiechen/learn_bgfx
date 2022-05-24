#include "PrimitiveType.h"
#include <json.hpp>
#include <unordered_map>
using json = nlohmann::json;
const std::unordered_map<std::string, PrimitiveType> s_primitiveTypes = { {"square", PrimitiveType::Square} };

namespace lb {
    struct Position {
        float x;
        float y;
        float z;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Position, x, y, z)

        struct Transform {
        Position position;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, position)

        struct Square {
        float size;
        std::string color;
        Transform transform;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Square, size, color, transform)
}

class GeometryLoader {
    static const std::unordered_map<std::string, PrimitiveType> s_primitiveTypes;
public:
    void loadConfigFile(const char* configFile, std::vector<lb::Square>& squares);
};
