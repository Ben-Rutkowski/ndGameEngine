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
    vec4 BLACK({0.0f, 0.0f, 0.0f, 1.0f});
    vec4 BLUE({0.3f, 0.3f, 0.35f, 1.0f});
    vec4 DBLUE({0.2f, 0.2f, 0.2f, 1.0f});

    mat4 view = camera.getView();
    mat4 proj = camera.getProj();
    vec4 camera_pos = camera.getPos();

    int N = meshes.size();

    // Draw Lines and points.
    // Write Every point to the depth buffer.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    x_line.drawPlane(view, proj);
    z_line.drawPlane(view, proj);

    for (int i=0; i<N; i++) {
        meshes[i].drawPoints(point_shader, view, proj, BLACK);
        meshes[i].drawLines(line_shader, view, proj, BLACK);
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
        meshes[i].drawFaces(face_shader, view, proj, camera_pos);
    }
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw faces with depth off on stenciled area
    glDepthFunc(GL_ALWAYS);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    for (int i=0; i<N; i++) {
        meshes[i].drawFaces(face_shader, view, proj, camera_pos);
    }
    glDisable(GL_BLEND);

    // Draw Lines and Points Occluded 
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    for (int i=0; i<N; i++) {
        meshes[i].drawPoints(point_shader, view, proj, DBLUE);
        meshes[i].drawLines(line_shader, view, proj, BLUE);
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
}