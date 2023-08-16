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
    Shader();
    Shader(ShaderType type_in, std::string file_path);

    unsigned int getID();

// --- Debugging ---
private:
    void checkCompile();
};



#endif