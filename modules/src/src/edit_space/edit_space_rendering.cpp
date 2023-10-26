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

void EditSpace::draw() {
    vec4 BLACK({0.0f, 0.0f, 0.0f, 1.0f});
    vec4 BLUE({0.3f, 0.3f, 0.35f, 1.0f});
    vec4 DBLUE({0.2f, 0.2f, 0.2f, 1.0f});
    vec4 ORANGE({0.8f, 0.6f, 0.3f, 1.0f});
    vec4 FACE({0.5f, 0.5f, 0.5f, 0.5f});

    emMaterial line_point_mat = {BLACK, BLACK, ORANGE};
    emMaterial back_point_mat = {DBLUE, DBLUE, ORANGE};
    emMaterial face_mat = {FACE, FACE};

    mat4 view = camera.getView();
    mat4 proj = camera.getProj();

    CameraPack cp = camera.getPack();

    // ==== DRAW ROUTINE ====

    window_frame.startDraw();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    int N = meshes.size();
    // Draw Lines and points.
    // Write Every point to the depth buffer.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    x_line.drawPlane(gridline_shader, gridline_plane_shader, view, proj);
    z_line.drawPlane(gridline_shader, gridline_plane_shader, view, proj);

    for (int i=0; i<N; i++) {
        // meshes[i].drawLines(line_shader, view, proj, BLACK, ORANGE);
        // meshes[i].drawPoints(point_shader, view, proj, BLACK, ORANGE);
        meshes[i].drawLines(line_shader, cp, line_point_mat);
        meshes[i].drawPoints(point_shader, cp, line_point_mat);
    }

    // Stencil face fragments with polygon offset
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDepthFunc(GL_LESS); 

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.5f, 1.0f);
    for (int i=0; i<N; i++) {
        meshes[i].drawFaces(face_shader, cp, face_mat);
    }
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw faces with depth off on stenciled area
    glDepthFunc(GL_ALWAYS);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i=0; i<N; i++) {
        meshes[i].drawFaces(face_shader, cp, face_mat);
    }
    glDisable(GL_BLEND);

    // Draw Lines and Points Occluded 
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    for (int i=0; i<N; i++) {
        meshes[i].drawPoints(point_shader, cp, back_point_mat);
        meshes[i].drawLines(line_shader, cp, back_point_mat);
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    if (scache[esLEFT_MOUSE]) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        select_box.drawFaces(select_box_face_shader);
        glDisable(GL_BLEND);

        select_box.drawLines(select_box_line_shader);
    }

    window_frame.endDraw();

    // debug_box.draw(camera.getView(), camera.getProj());
}

void EditSpace::drawFaceModeOld() {
    emMaterial line_point_material = {
        vec4({0.0f, 0.0f, 0.0f, 1.0f}), // Ambient Color
        vec4({0.2f, 0.2f, 0.2f, 1.0f}), // Diffuse Color
        vec4({0.8f, 0.6f, 0.3f, 1.0f})  // Select Color
    };
    emMaterial face_material = {
        vec4({0.2f, 0.15f, 0.15f, 1.0f}), // Ambient Color
        vec4({0.75f, 0.7f, 0.7f, 1.0f}),  // Diffuse Color
        vec4({1.0f, 1.0f, 1.0f, 1.0f})    // Select Color
    };
    CameraPack camera_pack = camera.getPack();

    // float WIDTH = 0.002f;
    float WIDTH = 0.003f;

    int N_mesh = meshes.size();

    window_frame.startDraw();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for (int i=0; i<N_mesh; i++) {
        meshes[i].drawPoints(fm_point_shader, camera_pack, line_point_material);
        meshes[i].drawLines(fm_point_shader, camera_pack, line_point_material);
        // meshes[i].drawLinesTEST(line_test_shader, camera_pack, line_point_material, WIDTH);
    }

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    for (int i=0; i<N_mesh; i++) {
        meshes[i].drawFaces(fm_face_shader, camera_pack, face_material);
    }
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_DEPTH_TEST);
    window_frame.endDraw();
}

void EditSpace::drawFaceMode() {
    emMaterial line_point_material = {
        vec4({0.0f, 0.0f, 0.0f, 1.0f}), // Ambient Color
        vec4({0.2f, 0.2f, 0.2f, 1.0f}), // Diffuse Color
        vec4({0.8f, 0.6f, 0.3f, 1.0f})  // Select Color
    };
    emMaterial face_material = {
        vec4({0.2f, 0.15f, 0.15f, 1.0f}), // Ambient Color
        vec4({0.75f, 0.7f, 0.7f, 1.0f}),  // Diffuse Color
        vec4({1.0f, 1.0f, 1.0f, 1.0f})    // Select Color
    };
    CameraPack camera_pack = camera.getPack();

    float WIDTH = 0.003f;
    int N_mesh = meshes.size();

    cull_frame.startDraw();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.0f, 1.0f);
    for (int i=0; i<N_mesh; i++) {
        meshes[i].drawFaces(fm_face_depth_shader, camera_pack, face_material);
    }
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_DEPTH_TEST);
    cull_frame.endDraw();

    window_frame.startDraw();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    for (int i=0; i<N_mesh; i++) {
        meshes[i].drawLinesTEST(line_test_shader, cull_frame, camera_pack, line_point_material, WIDTH);
    }   
    // cull_frame.draw(test_window_frame_shader);
    window_frame.endDraw();
}