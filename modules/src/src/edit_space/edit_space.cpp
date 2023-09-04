#include "edit_space.hpp"

Module EditSpace::module_name = Module::EDIT_SPACE;

EditSpace::EditSpace()
    :x_line( 
        vec4({1.0f, 0.0f ,0.0f, 1.0f}),
        vec4({0.2f, 0.8f, 0.2f, 1.0f})
    ),
    y_line( 
        vec4({0.0f, 1.0f ,0.0f, 1.0f}),
        vec4({0.2f, 0.2f, 0.8f, 1.0f})
    ),
    z_line( 
        vec4({0.0f, 0.0f ,1.0f, 1.0f}),
        vec4({0.8f, 0.2f, 0.2f, 1.0f})
    ),
    window_width{ 800.0f },
    window_height{ 600.0f },
    draw_select{ false } {

    point_shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"point.vs",
        EDIT_SPACE_SHADER_SUB_DIR"point.fs"
    );

    line_shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"line.vs",
        EDIT_SPACE_SHADER_SUB_DIR"line.fs"
    );

    face_shader.compileVF(
        EDIT_SPACE_SHADER_SUB_DIR"face.vs",
        EDIT_SPACE_SHADER_SUB_DIR"face.fs"
    );

    gridline_shader.compileVF(
        GRIDLINE_SHADER_SUB_DIR"gridline.vs",
        GRIDLINE_SHADER_SUB_DIR"gridline.fs"
    );

    gridline_plane_shader.compileVF(
        GRIDLINE_SHADER_SUB_DIR"gridline_plane.vs",
        GRIDLINE_SHADER_SUB_DIR"gridline.fs"
    );

    select_box_shader.compileVGF(
        SELECT_BOX_SHADER_SUB_DIR"select_box.vs",
        SELECT_BOX_SHADER_SUB_DIR"select_box.gs",
        SELECT_BOX_SHADER_SUB_DIR"select_box.fs"
    );

    select_box_faces_shader.compileVGF(
        SELECT_BOX_SHADER_SUB_DIR"select_box.vs",
        SELECT_BOX_SHADER_SUB_DIR"select_box_faces.gs",
        SELECT_BOX_SHADER_SUB_DIR"select_box_faces.fs"
    );

    setCallbacks();
}

void EditSpace::setManagerPtr(EventManager* ptr) {
    event_interface.linkManager(ptr);
}

EditMesh& EditSpace::ref(Id mesh_id) { return meshes[mesh_id]; }