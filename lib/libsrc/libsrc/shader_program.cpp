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

// === Uniforms ===
int ShaderProgram::uniformLocation(const char* name) {
    return glGetUniformLocation(program_id, name);
}

void ShaderProgram::uniform4f(int id, vec4 v) {
    glUniform4f(id, v[0], v[1], v[2], v[3]);
}

void ShaderProgram::uniformMat4f(int id, mat4 m) {
    glUniformMatrix4fv(id, 1, GL_TRUE, m.location());
}

// === Rendering ===
void ShaderProgram::use() {
    glUseProgram(program_id);
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