#include "debug_box.hpp"

DebugBox::DebugBox() {
    vertex_data[0] = vec4({-0.5f, -0.5f, 0.0f, 1.0f });
    vertex_data[1] = vec4({ 0.5f, -0.5f, 0.0f, 1.0f });
    vertex_data[2] = vec4({ 0.5f,  0.5f, 0.0f, 1.0f });
    vertex_data[3] = vec4({-0.5f,  0.5f, 0.0f, 1.0f });

    unsigned int indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0
    };

    vbi.bindAllBuffers();
    vbi.loadVerticesStream(&vertex_data[0], sizeof(vertex_data));
    vbi.loadIndicesStatic(&indices[0], sizeof(indices));
    vbi.configAttribf(0, 4, sizeof(vec4), (void*)0);
    vbi.unbindCurrent();

    program.compileVF(
        DEBUG_SHADER_SUB_DIR"debug_box.vs",
        DEBUG_SHADER_SUB_DIR"debug_box.fs"
    );
}

void DebugBox::setVerts(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    vertex_data[0] = v1;
    vertex_data[1] = v2;
    vertex_data[2] = v3;
    vertex_data[3] = v4;
    
    vbi.bindAllBuffers();
    vbi.editVertexData(&vertex_data[0], sizeof(vertex_data), 0);
    vbi.unbindCurrent();
}

void DebugBox::draw(mat4 view, mat4 proj) {
    program.use();
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    
    vbi.bindCurrent();
    vbi.drawElementsLines(8);
    vbi.unbindCurrent();
}