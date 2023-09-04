#include "select_box.hpp"

SelectBox::SelectBox()
    :end_points{ 
        vec2({0.2f, 0.2f}),
        vec2({0.6f, 0.6f})
    },
    indices{ 0, 1 } {

    vbi.bindAllBuffers();
    vbi.loadVerticesStream(&end_points[0], sizeof(end_points));
    vbi.loadIndicesStatic(&indices[0], sizeof(indices));
    vbi.configAttribf(0, 2, sizeof(vec2), (void*)0);
    vbi.unbindCurrent();
}

void SelectBox::grab(vec2 v, float width, float height) {
    end_points[0] = toClip(v, width, height);
    end_points[1] = toClip(v, width, height);

    vbi.bindAllBuffers();
    vbi.editVertexData(&end_points[0], sizeof(end_points), 0);
    vbi.unbindCurrent();
}

void SelectBox::drag(vec2 v, float width, float height) {
    end_points[1] = toClip(v, width, height);

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

vec2 SelectBox::xBounds() {
    return vec2({
        end_points[0][0], end_points[1][0]
    });
}

vec2 SelectBox::yBounds() {
    return vec2({
        end_points[0][1], end_points[1][1]
    });
}

vec2 SelectBox::toClip(vec2 v, float width, float height) {
    return vec2({
        2.0f*v[0]/width - 1.0f, 1.0f - 2.0f*v[1]/height
    });
}

void SelectBox::print() {
    std::cout << "Root: "; end_points[0].print();
    std::cout << "End: "; end_points[1].print();
}