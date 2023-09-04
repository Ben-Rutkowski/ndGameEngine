#include "vertex_buffer_interface.hpp"

// === Constructors and Initialization ===
VertexBufferInterface::VertexBufferInterface() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

// === Private ===
void VertexBufferInterface::bindAllBuffers() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void VertexBufferInterface::unbindCurrent() {
    glBindVertexArray(0);
}

void VertexBufferInterface::bindCurrent() {
    glBindVertexArray(vao);
}

void VertexBufferInterface::loadVerticesStatic(void* data, int size) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferInterface::loadVerticesStream(void* data, int size) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
}

void VertexBufferInterface::loadIndicesStatic(void* data, int size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferInterface::loadIndicesStream(void* data, int size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
}

void VertexBufferInterface::editVertexData(void* data, int size, int buffer_offset) {
    glBufferSubData(GL_ARRAY_BUFFER, buffer_offset, size, data);
}

void VertexBufferInterface::configAttribf(int attrib_index, int size, int stride, void* offeset) {
    glVertexAttribPointer(attrib_index, size, GL_FLOAT, GL_FALSE, stride, offeset);
    glEnableVertexAttribArray(attrib_index);
}

void VertexBufferInterface::drawElementsTriangles(int size) {
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

void VertexBufferInterface::drawElementsLines(int size) {
    glDrawElements(GL_LINES, size, GL_UNSIGNED_INT, 0);
}

void VertexBufferInterface::drawPoints(int size) {
    glDrawArrays(GL_POINTS, 0, size);
}

// void VertexBufferInterface::drawElementsStatic(int size) {
//     glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
// }