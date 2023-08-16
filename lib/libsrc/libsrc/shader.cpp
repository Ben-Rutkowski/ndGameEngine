#include "shader.hpp"
#include "file_utils.hpp"

#include <iostream>

Shader::Shader(ShaderType type_in, std::string file_path)
    :type{ type_in }, GL_type{ getGLType(type_in) } {

    std::string shader   = futil::readRelativeFile(file_path, ROOT);
    const char* shader_c = shader.c_str();

    shader_id = glCreateShader(getGLType(type));
    glShaderSource(shader_id, 1, &shader_c, NULL);
    glCompileShader(shader_id);
    checkCompile();
}

// === Debugging ===
void Shader::checkCompile() {
    int  success;
    char info[512];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info);
        std::cout << "Shader Failed To Compile ShaderType:";
        std::cout << (int)type << " " << info << std::endl;
    }
}

// === Static ===
ShaderEnumMap Shader::enum_map = enumUtil::getShaderEnumMap();
GLenum Shader::getGLType(ShaderType type) {
    return enum_map.get(type);
}