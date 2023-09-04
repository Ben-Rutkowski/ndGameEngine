#include "select_box.hpp"

SelectBox::SelectBox()
    :end_points{ vec2(0.0f), vec2(0.0f) },
    indices{ 0, 1 } {

    vbi.bindAllBuffers();
    vbi.loadVerticesStream(&end_points, sizeof(end_points));
    vbi.loadIndicesStatic(&indices, sizeof(indices));
    vbi.configAttribf(0, 2, sizeof(vec2), (void*)0);
    vbi.unbindCurrent();
}

void SelectBox::grab(vec2 v) {
    end_points[0] = v;
    end_points[1] = v;

    vbi.bindCurrent();
    vbi.editVertexData(&end_points, sizeof(end_points), 0);
    vbi.unbindCurrent();
}

void SelectBox::drag(vec2 v) {
    end_points[1] = v;

    vbi.bindCurrent();
    vbi.editVertexData(&end_points, sizeof(vec2), sizeof(vec2));
    vbi.unbindCurrent();
}

void SelectBox::draw(ShaderProgram& program) {
    program.use();
    vbi.bindCurrent();
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