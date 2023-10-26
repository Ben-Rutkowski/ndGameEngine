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

    window_frame.load();

    // TAKE OUT v
    cull_frame.load();
}

void EditSpace::drawFaceMode() {
    // emMaterial line_point_material = {
    //     vec4({0.0f, 0.0f, 0.0f, 1.0f}), // Ambient Color
    //     vec4({0.2f, 0.2f, 0.2f, 1.0f}), // Diffuse Color
    //     vec4({0.8f, 0.6f, 0.3f, 1.0f})  // Select Color
    // };
    // emMaterial face_material = {
    //     vec4({0.2f, 0.15f, 0.15f, 1.0f}), // Ambient Color
    //     vec4({0.75f, 0.7f, 0.7f, 1.0f}),  // Diffuse Color
    //     vec4({1.0f, 1.0f, 1.0f, 1.0f})    // Select Color
    // };
    // CameraPack camera_pack = camera.getPack();

    // float WIDTH = 0.003f;
    // int N_mesh = meshes.size();
}