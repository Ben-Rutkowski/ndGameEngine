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

// --- Binding ---
public:
    void bindAllBuffers();
    void unbindCurrent();
    void bindCurrent();

// --- Loading ---
    void loadVerticesStatic(void* data, int size);
    void loadVerticesStream(void* data, int size);
    void loadIndicesStatic(void* data, int size);
    void loadIndicesStream(void* data, int size);
    void editVertexData(void* data, int size, int buffer_offset);

// --- Config ---
    void configAttribf(int attrib_index, int size, int stride, void* offeset);
    void configAttribi(int attrib_index, int size, int stride, void* offeset);
    void configAttribui(int attrib_index, int size, int stride, void* offeset);
    void configAttribb(int attrib_index, int size, int stride, void* offeset);

// --- Drawing ---
    void drawElementsTriangles(int size);
    void drawElementsLines(int size);
    void drawPoints(int size);
};

#endif