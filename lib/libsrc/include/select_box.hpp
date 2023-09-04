#ifndef SELECT_BOX_HPP
#define SELECT_BOX_HPP

#include "vertex_buffer_interface.hpp"

class SelectBox {
private:
    VertexBufferInterface vbi;

    vec2 end_points[2]; // Root, End

    unsigned int indices[2];

public:
    SelectBox();

    void grab(vec2 v, float width, float height);
    void drag(vec2 v, float width, float height);
    void drawLines(ShaderProgram& line_shader);
    void drawFaces(ShaderProgram& face_shader);
    vec2 xBounds();
    vec2 yBounds();

// --- Private ---
vec2 toClip(vec2 v, float width, float height);

// --- Debugging ---
public:
    void print();
};

#endif