#include "ConfigLoadResult.h"
#include "JsonDefinitions.h"
#include "LightType.h"
#include "PrimitiveType.h"
#include <unordered_map>

// For debug printing to std::cout
std::ostream& operator<<(std::ostream& os, const lb::Transform& t);

class GeometryLoader {
    static const std::unordered_map<std::string, PrimitiveType> s_primitiveTypes;
    static const std::unordered_map<std::string, LightType> s_lightTypes;
public:
    void loadConfigFile(const char* configFile, ConfigLoadResult& configLoadResult);
};
