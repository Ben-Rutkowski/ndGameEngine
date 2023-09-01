#include "edit_space.hpp"

void EditSpace::load() {
    x_line.load(50.0f);
    y_line.load(50.0f);
    z_line.load(50.0f);

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
        meshes[i].draw(point_shader, line_shader, face_shader, view, proj);
    }

    glEnable(GL_DEPTH_TEST);
    x_line.draw(view, proj);
    y_line.draw(view, proj);
    z_line.draw(view, proj);
    // glDisable(GL_DEPTH_TEST);
}