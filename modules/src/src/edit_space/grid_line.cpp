#include "edit_space.hpp"

GridLine::GridLine(vec4 dir_in, vec4 color_in)
    :direction{ dir_in },
    color{ color_in } {

    shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"gridline.vs",
        EDIT_SPACE_SHADER_SUB_DIR"gridline.fs"
    );
}

void GridLine::load(float length) {
    vec4 vertices[4] = {
        direction.multK(length, 3), 
        color,
        direction.multK(-length, 3), 
        color
    };

    unsigned int indices[2] = { 0, 1 };

    vbi.bindAllBuffers();
    vbi.loadVerticesStatic(&vertices[0], sizeof(vertices));
    vbi.loadIndicesStatic(&indices[0], sizeof(indices));
    vbi.configAttribf(0, 4, 2*sizeof(vec4), (void*)0);
    vbi.configAttribf(1, 4, 2*sizeof(vec4), (void*)(sizeof(vec4)));
    vbi.unbindCurrent();
}

void GridLine::draw(mat4 view, mat4 proj) {
    shader.use();
    int view_id = shader.uniformLocation("view");
    int proj_id = shader.uniformLocation("proj");
    shader.uniformMat4f(view_id, view);
    shader.uniformMat4f(proj_id, proj);
    vbi.bindCurrent();
    glLineWidth(10);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    vbi.unbindCurrent();
}