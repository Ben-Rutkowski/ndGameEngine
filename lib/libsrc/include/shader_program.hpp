#include "shader.hpp"
#include "matrix.hpp"

#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

typedef EnumMap<ShaderType, Shader, SHADER_TYPE_LEN> ShaderArray;

class ShaderProgram {
private:
    unsigned int program_id;

    ShaderArray array;

public:
    ShaderProgram();

    void attachShader(ShaderType type, Shader shader);
    void linkProgram();

    void compileVF(std::string vs_rel_path, std::string fs_rel_path);

// --- Uniforms ---
public:
    int  uniformLocation(const char* name);
    void uniform4f(const char* name, vec4 v);
    void uniformMat4f(const char*, mat4 m);
    void uniform4f(int id, vec4 v);
    void uniformMat4f(int id, mat4 m);

// --- Rendering ---
public:
    void use();

// --- Debugging ---
private:
    void checkLink();

// --- Static ---
public:
    static ShaderProgram VF(std::string vs_rel_path, std::string fs_rel_path);
};

#endif