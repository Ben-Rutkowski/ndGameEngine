#include "select_box.hpp"

SelectBox::SelectBox()
    :root({0.0f, 0.0f}),
    end({0.0f, 0.0f}) {

    unsigned int line_indices[] = { 0, 1, 1, 3, 3, 2, 2, 0 };
    unsigned int face_indices[] = { 0, 1, 3, 3, 2, 0 };

    setGrabVData(root);

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

void SelectBox::grab(vec2 v) {
    setGrabVData(v);

    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    face_vbi.unbindCurrent();
}

void SelectBox::drag(vec2 v) {
    setDragVData(v);

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

void SelectBox::print() {
    std::cout << "Root: "; root.print();
    std::cout << "End: "; end.print();
}