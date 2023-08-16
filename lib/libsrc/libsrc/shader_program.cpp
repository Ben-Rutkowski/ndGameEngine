#include "shader_program.hpp"

#include <iostream>

ShaderProgram::ShaderProgram() {
    program_id = glCreateProgram();
}

void ShaderProgram::attachShader(ShaderType type, Shader shader) {
    array.addEntry(type, shader);
    glAttachShader(program_id, shader.getID());
}

void ShaderProgram::linkProgram() {
    glLinkProgram(program_id);
    checkLink();
}

// === Debugging ===
void ShaderProgram::checkLink() {
    int  success;
    char info[512];

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info);
        std::cout << "Failed to link program: " << info << std::endl;
    }
}