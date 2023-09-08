#include "select_box.hpp"

SelectBox::SelectBox()
    :end_points{ 
        vec2({0.2f, 0.2f}),
        vec2({0.6f, 0.6f})
    },
    indices{ 0, 1 },
    select_mat{ mat4::iden() } {

    vbi.bindAllBuffers();
    vbi.loadVerticesStream(&end_points[0], sizeof(end_points));
    vbi.loadIndicesStatic(&indices[0], sizeof(indices));
    vbi.configAttribf(0, 2, sizeof(vec2), (void*)0);
    vbi.unbindCurrent();
}

void SelectBox::grab(vec2 v, float width, float height) {
    end_points[0] = toClip(v, width, height);
    end_points[1] = toClip(v, width, height);
    // end_point = toClip(v, width, height);

    vbi.bindAllBuffers();
    vbi.editVertexData(&end_points[0], sizeof(end_points), 0);
    vbi.unbindCurrent();
}

void SelectBox::drag(vec2 v, float width, float height) {
    end_points[1] = toClip(v, width, height);
    // end_point     = toClip(v, width, height);

    vbi.bindAllBuffers();
    vbi.editVertexData(&end_points[1], sizeof(vec2), sizeof(vec2));
    vbi.unbindCurrent();
}

void SelectBox::drawLines(ShaderProgram& line_shader) {
    vbi.bindCurrent();
    line_shader.use();
    vbi.drawElementsLines(2);
    vbi.unbindCurrent();
}

void SelectBox::drawFaces(ShaderProgram& face_shader) {
    vbi.bindCurrent();
    face_shader.use();
    vbi.drawElementsLines(2);
    vbi.unbindCurrent();
}

vec2 SelectBox::getBR() {
    return vec2({
        std::min(end_points[0][0], end_points[1][0]),
        std::min(end_points[0][1], end_points[1][1])
    });
}

vec2 SelectBox::getTL() {
    return vec2({
        std::max(end_points[0][0], end_points[1][0]),
        std::max(end_points[0][1], end_points[1][1])
    });
}

float SelectBox::getXD() {
    return abs(end_points[0][0] - end_points[1][0]);
}

float SelectBox::getYD() {
    return abs(end_points[0][1] - end_points[1][1]);
}

void SelectBox::setSelectMatProj(mat4 mat) {
    select_mat = mat;
}

bool SelectBox::checkSelect(vec4 point) {
    vec4 p = select_mat*point;
    float s = 1.0f/p[2];
    float x = p[0]*s;
    float y = p[1]*s;

    return  s>0.0f && x>=0.0f && y>=0.0f && x <= 1.0f && y <= 1.0f;
}

vec2 SelectBox::toClip(vec2 v, float window_width, float window_height) {
    return vec2({
        2.0f*v[0]/window_width - 1.0f, 1.0f - 2.0f*v[1]/window_height
    });
}

void SelectBox::print() {
    std::cout << "BR: "; getBR().print();
    std::cout << "TL: "; getTL().print();
}