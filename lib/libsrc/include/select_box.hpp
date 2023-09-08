#ifndef SELECT_BOX_HPP
#define SELECT_BOX_HPP

#include "vertex_buffer_interface.hpp"

class SelectBox {
private:
    VertexBufferInterface vbi;

    vec2 end_points[2]; // Root, End

    unsigned int indices[2];

    mat4 select_mat;

public:
    SelectBox();

    void grab(vec2 v, float width, float height);
    void drag(vec2 v, float width, float height);
    void drawLines(ShaderProgram& line_shader);
    void drawFaces(ShaderProgram& face_shader);
    vec2 getBR();
    vec2 getTL();
    // float getXD();
    // float getYD();

    // void setSelectMatProj(mat4 mat);
    // bool checkSelect(vec4 point);    

// --- Private ---
vec2 toClip(vec2 v, float window_width, float window_height);

// --- Debugging ---
public:
    void print();
    mat4 getSelect() { return select_mat; }
};

#endif