#include "rendering/shader.hpp"
#include "file_utils.hpp"

#include <iostream>

Shader::Shader() 
    :shader_id{ 0 },
    type{ ShaderType::null },
    GL_type{ 0 } {}

Shader::Shader(ShaderType type_in, std::string file_path)
    :type{ type_in },
    GL_type{ EnumMapIndex::glShader(type_in) } {

    std::string shader   = futil::readRelativeFile(file_path, ROOT);
    const char* shader_c = shader.c_str();

    shader_id = glCreateShader(GL_type);
    glShaderSource(shader_id, 1, &shader_c, NULL);
    glCompileShader(shader_id);
    checkCompile();
}

unsigned int Shader::getID() {
    return shader_id;
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