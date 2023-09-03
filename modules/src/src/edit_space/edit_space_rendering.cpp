#include "edit_space.hpp"

void EditSpace::load() {
    vec4 BLANK_COLOR({0.4f, 0.4f, 0.4f, 1.0f});

    x_line.load(10.0f);
    x_line.createPlane(
        BLANK_COLOR,
        vec4({0.0f, 0.0f, 1.0f, 1.0f})
    );
    z_line.load(10.0f);
    z_line.createPlane(
        BLANK_COLOR,
        vec4({1.0f, 0.0f, 0.0f, 1.0f})
    );
    y_line.load(5.0f);

    int N = meshes.size();
    for (int i=0; i<N; i++) {
        meshes[i].load();
    }
}

void EditSpace::draw() {
    mat4 view = camera.getView();
    mat4 proj = camera.getProj();

    int N = meshes.size();
    for (int i=0; i<N; i++) {
        meshes[i].draw(point_shader, line_shader, face_shader, view, proj, camera.getPos());
    }

    glEnable(GL_DEPTH_TEST);;
    x_line.drawPlane(view, proj);
    z_line.drawPlane(view, proj);
    y_line.draw(view, proj);
}