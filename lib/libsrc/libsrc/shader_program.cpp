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

void ShaderProgram::compileVF(std::string vs_rel_path, std::string fs_rel_path) {
    Shader vs(ShaderType::VERTEX, vs_rel_path);
    Shader fs(ShaderType::FRAGMENT, fs_rel_path);
    attachShader(ShaderType::VERTEX, vs);
    attachShader(ShaderType::FRAGMENT, fs);
    linkProgram();
}

// === Uniforms ===
int ShaderProgram::uniformLocation(const char* name) {
    return glGetUniformLocation(program_id, name);
}

// void ShaderProgram::uniform4f(int id, vec4 v) {
//     glUniform4f(id, v[0], v[1], v[2], v[3]);
// }

// void ShaderProgram::uniformMat4f(int id, mat4 m) {
//     glUniformMatrix4fv(id, 1, GL_TRUE, m.location());
// }

void ShaderProgram::uniform1f(const char* name, float c) {
    int id = glGetUniformLocation(program_id, name);
    glUniform1f(id, c);
}

void ShaderProgram::uniform2f(const char* name, vec2 v) {
    int id = glGetUniformLocation(program_id, name);
    glUniform2f(id, v[0], v[1]);
}

void ShaderProgram::uniform3f(const char* name, vec3 v) {
    int id = glGetUniformLocation(program_id, name);
    glUniform3f(id, v[0], v[1], v[2]);
}

void ShaderProgram::uniform4f(const char* name, vec4 v) {
    int id = glGetUniformLocation(program_id, name);
    glUniform4f(id, v[0], v[1], v[2], v[3]);
}

void ShaderProgram::uniformMat4f(const char* name, mat4 m) {
    int id = glGetUniformLocation(program_id, name);
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

// === Static ===
ShaderProgram ShaderProgram::VF(std::string vs_rel_path, std::string fs_rel_path) {
    ShaderProgram program;
    Shader vs(ShaderType::VERTEX, vs_rel_path);
    Shader fs(ShaderType::FRAGMENT, fs_rel_path);
    program.attachShader(ShaderType::VERTEX, vs);
    program.attachShader(ShaderType::FRAGMENT, fs);
    program.linkProgram();
    return program;
}