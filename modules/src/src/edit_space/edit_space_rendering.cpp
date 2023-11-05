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

    // TAKE OUT
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

    /*
        1. Draw faces onto cull_frame texture using a polygon offset.
        2. Draw points and lines by culling hidden points, sampling from cull_frame
            drawing to the z-buffer.
        3. Draw faces with depth test.
    */

    // 1. Draw faces to cull_frame
    // cull_frame.bindDraw();
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    // glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);
    // glEnable(GL_DEPTH_TEST);

    // for (int i=0; i<N_mesh; i++) {
    //     meshes[0].drawFaces(fm_face_depth_shader, camera_pack, face_material);
    // }

    // glDisable(GL_DEPTH_TEST);
    // cull_frame.unbind();

    // window_frame.bindDraw();
    // glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    // for (int i=0; i<N_mesh; i++) {
    //     meshes[0].drawPointsTEST(fm_point_shader, cull_frame, camera_pack, line_point_material);
    // }
    // window_frame.unbind();
}