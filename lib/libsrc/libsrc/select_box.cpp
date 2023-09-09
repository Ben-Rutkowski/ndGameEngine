#include "select_box.hpp"

SelectBox::SelectBox()
    :end_points{vec2({0.2f, 0.2f}), vec2({0.6f, 0.6f})},
    root({0.0f, 0.2f}), end({0.0f, 0.0f}) {

    setGrabVData(root);
    unsigned int line_indices[] = {
        0, 1, 1, 3, 3, 2, 2, 0
    };

    unsigned int face_indices[] = {
        0, 1, 3, 3, 2, 0
    };

    line_vbi.bindAllBuffers();
    line_vbi.loadVerticesStream(&vertex_data[0], sizeof(vertex_data));
    line_vbi.loadIndicesStream(&line_indices, sizeof(line_indices));
    line_vbi.configAttribf(0, 2, sizeof(vec2), (void*)0);
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.loadVerticesStream(&vertex_data[0], sizeof(vertex_data));
    face_vbi.loadIndicesStream(&face_indices, sizeof(face_indices));
    face_vbi.configAttribf(0, 2, sizeof(vec2), (void*)0);
    face_vbi.unbindCurrent();
}

void SelectBox::grab(vec2 v, float width, float height) {
    end_points[0] = toClip(v, width, height);
    end_points[1] = toClip(v, width, height);

    setGrabVData(toClip(v, width, height));

    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    face_vbi.unbindCurrent();
}

void SelectBox::drag(vec2 v, float width, float height) {
    end_points[1] = toClip(v, width, height);

    setDragVData(toClip(v, width, height));

    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    face_vbi.unbindCurrent();
}

void SelectBox::drawLines(ShaderProgram& program) {
    line_vbi.bindCurrent();
    program.use();
    line_vbi.drawElementsLines(8);
    line_vbi.unbindCurrent();
}

void SelectBox::drawFaces(ShaderProgram& program) {
    face_vbi.bindCurrent();
    program.use();
    face_vbi.drawElementsTriangles(6);
    face_vbi.unbindCurrent();
}

vec2 SelectBox::getRoot() { return root; }
vec2 SelectBox::getEnd()  { return end; }

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

void SelectBox::setGrabVData(vec2 v) {
    root = v;
    vertex_data[0] = root;
    vertex_data[1] = root;
    vertex_data[2] = root;
    vertex_data[3] = root;
}

void SelectBox::setDragVData(vec2 v) {
    end = v;
    vertex_data[0] = root;
    vertex_data[1] = vec2({end[0],  root[1]});
    vertex_data[2] = vec2({root[0], end[1]});
    vertex_data[3] = end;
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