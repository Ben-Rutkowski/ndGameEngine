#include "common.hpp"

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader {
// --- Attibutes ---
private:
    unsigned int shader_id;
    ShaderType   type;
    GLenum       GL_type;

public:
    Shader(ShaderType type_in, std::string file_path);

// --- Debugging ---
private:
    void checkCompile();

// --- Static ---
private:
    static ShaderEnumMap enum_map;
    static GLenum getGLType(ShaderType type);
};



#endif