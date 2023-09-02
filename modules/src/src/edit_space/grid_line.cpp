#include "edit_space.hpp"

GridLine::GridLine(vec4 dir_in, vec4 color_in)
    :direction{ dir_in },
    color{ color_in } {

    shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"gridline.vs",
        EDIT_SPACE_SHADER_SUB_DIR"gridline.fs"
    );

    plane_shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"gridline_plane.vs",
        EDIT_SPACE_SHADER_SUB_DIR"gridline.fs"
    );
}

void GridLine::load(float length_in) {
    length = length_in;

    vec4 vertices[4] = {
        direction.multK(length, 3), 
        direction.multK(-length, 3), 
    };

    unsigned int indices[2] = { 0, 1 };

    vbi.bindAllBuffers();
    vbi.loadVerticesStatic(&vertices[0], sizeof(vertices));
    vbi.loadIndicesStatic(&indices[0], sizeof(indices));
    vbi.configAttribf(0, 4, sizeof(vec4), (void*)0);
    vbi.unbindCurrent();
}

void GridLine::draw(mat4 view, mat4 proj) {
    shader.use();
    shader.uniformMat4f("view", view);
    shader.uniformMat4f("proj", proj);
    shader.uniform4f("color", color);
    vbi.bindCurrent();
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    vbi.unbindCurrent();
}

void GridLine::drawPlane(mat4 view, mat4 proj) {
    draw(view, proj);

    plane_shader.use();
    plane_shader.uniformMat4f("view", view);
    plane_shader.uniformMat4f("proj", proj);
    plane_shader.uniform4f("color", blank_color);
    plane_shader.uniform4f("orthog", orthog);

    for (int i=0; i<10; i++) {
        plane_shader.uniform1f(
            ("offsets[" + std::to_string(i) + "]").c_str(),
            length*offsets[i]
        );
    }

    vbi.bindCurrent();
    glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, 0, 10);
    vbi.unbindCurrent();
}

void GridLine::createPlane(vec4 blank_in, vec4 othog_in) {
    blank_color = blank_in;
    orthog       = othog_in;

    for (int i=0; i<5; i++) {
        offsets[2*i]   = (float)(i+1)/5.0f;
        offsets[2*i+1] = -(float)(i+1)/5.0f;
    }
}