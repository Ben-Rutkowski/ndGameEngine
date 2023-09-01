#include "edit_space.hpp"

void EditSpace::load() {
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
}