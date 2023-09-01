#include "edit_space.hpp"

Module EditSpace::module_name = Module::EDIT_SPACE;

EditSpace::EditSpace()
    :x_line( 
        vec4({1.0f, 0.0f ,0.0f, 1.0f}),
        vec4({0.2f, 0.9f, 0.2f, 1.0f})
    ),
    y_line( 
        vec4({0.0f, 1.0f ,0.0f, 1.0f}),
        vec4({0.2f, 0.2f, 0.9f, 1.0f})
    ),
    z_line( 
        vec4({0.0f, 0.0f ,1.0f, 1.0f}),
        vec4({0.9f, 0.2f, 0.2f, 1.0f})
    ) {

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

    setCallbacks();
}

void EditSpace::setManagerPtr(EventManager* ptr) {
    event_interface.linkManager(ptr);
}

EditMesh& EditSpace::ref(Id mesh_id) { return meshes[mesh_id]; }