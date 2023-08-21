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
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferInterface::bindCurrent() {
    glBindVertexArray(vao);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBufferInterface::configAttribf(int attrib_index, int size, int stride, int offeset) {
    glVertexAttribPointer(attrib_index, size, GL_FLOAT, GL_FALSE, stride, (void*)offeset);
    glEnableVertexAttribArray(attrib_index);
}

void VertexBufferInterface::loadVerticesStatic(void* data, int size) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferInterface::loadIndicesStatic(void* data, int size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferInterface::drawElementsStatic(int size) {
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}