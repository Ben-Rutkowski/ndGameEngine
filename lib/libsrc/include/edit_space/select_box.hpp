#ifndef SELECT_BOX_HPP
#define SELECT_BOX_HPP

#include "rendering/vertex_buffer_interface.hpp"

class SelectBox {
private:
    VertexBufferInterface line_vbi;
    VertexBufferInterface face_vbi;

    vec2 root, end;
    vec2 vertex_data[4];

public:
    SelectBox();

    vec2 getRoot();
    vec2 getEnd();

    void grab(vec2 v);
    void drag(vec2 v);
    void drawLines(ShaderProgram& program);
    void drawFaces(ShaderProgram& program);

// --- Private ---
private:
    void setGrabVData(vec2 v);
    void setDragVData(vec2 v);

// --- Debugging ---
public:
    void print();
};

#endif