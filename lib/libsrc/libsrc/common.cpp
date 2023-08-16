#include "common.hpp"

ShaderEnumMap enumUtil::getShaderEnumMap() {
    ShaderEnumMap map;
    map.addEntry(ShaderType::VERTEX,   GL_VERTEX_SHADER);
    map.addEntry(ShaderType::FRAGMENT, GL_FRAGMENT_SHADER);
    map.addEntry(ShaderType::GEOMETRY, GL_GEOMETRY_SHADER);

    return map;
}