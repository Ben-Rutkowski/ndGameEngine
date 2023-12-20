#include "rendering/shader.hpp"
#include "math/matrix.hpp"

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
    void compileVGF(std::string vs_rel_path, std::string gs_rel_path, std::string fs_rel_path);

// --- Uniforms ---
public:
    int  uniformLocation(const char* name);
    void uniform1i(const char* name, int c);
    void uniform1f(const char* name, float c);
    void uniform2f(const char* name, vec2 v);
    void uniform3f(const char* name, vec3 v);
    void uniform4f(const char* name, vec4 v);
    void uniformMat4f(const char*, mat4 m);

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