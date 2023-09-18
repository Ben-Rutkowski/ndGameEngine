#include "edit_space.hpp"

// Module EditSpace::module_name = Module::EDIT_SPACE;

EditSpace::EditSpace()
    // :ndModuleOld(Module::EDIT_SPACE, esLEN),
    :ndModuleInstance<EditSpace>(Module::EDIT_SPACE, esLEN),
    x_line( 
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
    )  {

    dcache.ww = 800.0f;
    dcache.wh = 600.0f;

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

    select_box_line_shader.compileVF(
        SELECT_BOX_SHADER_SUB_DIR"select_box_lines.vs",
        SELECT_BOX_SHADER_SUB_DIR"select_box_lines.fs"
    );

    select_box_face_shader.compileVF(
        SELECT_BOX_SHADER_SUB_DIR"select_box_faces.vs",
        SELECT_BOX_SHADER_SUB_DIR"select_box_faces.fs"
    );

    setCallbacks();
}

// void EditSpace::setManagerPtr(EventManagerOld* ptr) {
//     event_interface.linkManager(ptr);
// }

EditMesh& EditSpace::ref(Id mesh_id) { return meshes[mesh_id]; }