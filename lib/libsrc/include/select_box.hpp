#ifndef SELECT_BOX_HPP
#define SELECT_BOX_HPP

#include "vertex_buffer_interface.hpp"

class SelectBox {
private:
    VertexBufferInterface vbi;
    VertexBufferInterface line_vbi;
    VertexBufferInterface face_vbi;

    vec2 end_points[2]; // Root, End

    vec2 root, end;
    vec2 vertex_data[4];

public:
    SelectBox();

    void grab(vec2 v, float width, float height);
    void drag(vec2 v, float width, float height);
    void drawLines(ShaderProgram& program);
    void drawFaces(ShaderProgram& program);
    vec2 getBR();
    vec2 getTL();   
    vec2 getRoot();
    vec2 getEnd();

// --- Private ---
private:
    vec2 toClip(vec2 v, float window_width, float window_height);
    void setGrabVData(vec2 v);
    void setDragVData(vec2 v);

// --- Debugging ---
public:
    void print();
};

#endif