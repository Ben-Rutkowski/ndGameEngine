#include "edit_space.hpp"

GridLine::GridLine(vec4 dir_in, vec4 color_in)
    :direction{ dir_in },
    color{ color_in } {}

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

void GridLine::draw(ShaderProgram& program, mat4 view, mat4 proj) {
    program.use();
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    program.uniform4f("color", color);
    vbi.bindCurrent();
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    vbi.unbindCurrent();
}

void GridLine::drawPlane(ShaderProgram& program, ShaderProgram& plane_program, mat4 view, mat4 proj) {
    draw(program, view, proj);

    plane_program.use();
    plane_program.uniformMat4f("view", view);
    plane_program.uniformMat4f("proj", proj);
    plane_program.uniform4f("color", blank_color);
    plane_program.uniform4f("orthog", orthog);

    for (int i=0; i<10; i++) {
        plane_program.uniform1f(
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
    orthog      = othog_in;

    for (int i=0; i<5; i++) {
        offsets[2*i]   = (float)(i+1)/5.0f;
        offsets[2*i+1] = -(float)(i+1)/5.0f;
    }
}