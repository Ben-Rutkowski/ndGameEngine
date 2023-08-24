#include "shader_program.hpp"

#ifndef VERTEX_BUFFER_INTERFACE_HPP
#define VERTEX_BUFFER_INTERFACE_HPP

/*

When loading static data, you must
    - bind all buffers
    - load vertex and index data
    - config vao pointers
    - unbind current

When drawing static, you must
    - use program
    - bind current
    - DRAW CALLS
    - unbind current

*/

class VertexBufferInterface {
// --- Attributes ---
protected:
    unsigned int vbo, vao, ebo;

// --- Constructors and Initialization ---
public:
    VertexBufferInterface();

// --- Rendering ---
public:
    virtual void loadData() = 0;
    virtual void draw(ShaderProgram& program) = 0;

// --- Private ---
protected:
    void bindAllBuffers();
    void unbindCurrent();
    void bindCurrent();
    void configAttribf(int attrib_index, int size, int stride, void* offeset);
    void loadVerticesStatic(void* data, int size);
    void loadIndicesStatic(void* data, int size);
    void drawElementsStatic(int size);
};

#endif