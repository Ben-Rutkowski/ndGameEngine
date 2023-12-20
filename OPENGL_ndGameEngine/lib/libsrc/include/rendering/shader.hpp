#include "common.hpp"

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

/* CLASS Shader

A Shader object creates shaders in OpenGL. Each Shader contains the
ID of a particular shader in OpenGL.

======== ATTRIBUTES ========
- unsigned int shader_id
- ShaderType   type
- GLenum       GL_type


======== METHODS ========
-------- CONSTRUCTORS --------
- Shader : Creates a null shader object, does not use OpenGL

- Shader : Creates a shader in OpenGL
    ==Parameters==
        - ShaderType type_in : The type of shader (VERTEX, FRAGMENT, ...)
        - string file_path : The relaltive path of the shader code.

-------- GETS AND SETS--------
- getID : Returns the OpenGL id of the shader.
    ==Return==
        - unsigned int

-------- DEBUGGING --------
- checkCompile : If shader failed to compile, prints info log   

*/

class Shader {
// --- Attibutes ---
private:
    unsigned int shader_id;
    ShaderType   type;
    GLenum       GL_type;

// --- Constructors ---
public:
    Shader();
    Shader(ShaderType type_in, std::string file_path);

// --- Gets and Sets ---
public:
    unsigned int getID();

// --- Debugging ---
private:
    void checkCompile();
};



#endif