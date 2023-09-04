#ifndef SELECT_BOX_HPP
#define SELECT_BOX_HPP

#include "vertex_buffer_interface.hpp"

class SelectBox {
private:
    VertexBufferInterface vbi;

    vec2         end_points[2]; // Root, End
    unsigned int indices[2];

public:
    SelectBox();

    void grab(vec2 v);
    void drag(vec2 v);
    void draw(ShaderProgram& program);
    vec2 xBounds();
    vec2 yBounds();
};

#endif