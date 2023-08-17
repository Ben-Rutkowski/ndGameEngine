#include "shader.hpp"

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

// --- Rendering ---
public:
    void use();

// --- Debugging ---
private:
    void checkLink();
};

#endif