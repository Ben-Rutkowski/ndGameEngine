#ifndef GRIDLINE_HPP
#define GRIDLINE_HPP

#include "rendering/vertex_buffer_interface.hpp"
#include "math/vector.hpp"

/* CLASS GridLine 
*/

class GridLine {
private:
    VertexBufferInterface vbi;

    float length;
    vec4 direction;
    vec4 color;

    float offsets[10];
    vec4  blank_color;
    vec4  orthog;

public:
    GridLine(vec4 dir_in, vec4 color_in);
    void load(float length_in);
    void draw(ShaderProgram& program, mat4 view, mat4 proj);
    void drawPlane(ShaderProgram& program, ShaderProgram& plane_program, mat4 view, mat4 proj);
    void createPlane(vec4 blank_in, vec4 othog_in);
};

#endif