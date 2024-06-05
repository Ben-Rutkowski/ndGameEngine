#include "draw_routine_indices.h"
#include "edit_space.hpp"
#define __INTERNAL__
#include "math/matrix.hpp"
#include "shader_types/S_thick_line_types.h"

void ndEditSapceModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::DEBUG:          onDebug(event); break;
    case Operation::BEGIN_START_UP: onBeginStartUp(event); break;
    case Operation::END_START_UP:   onEndStartUp(event); break;
    case Operation::START_FRAME:    onStartFrame(event); break;
    case Operation::END_FRAME:      onEndFrame(event); break;
    case Operation::DRAW:           onDraw(event); break;
    case Operation::RESIZE_FRAME:   onResizeFrame(event); break;
    default: break;
    }
}


// ================ Debug ================
void ndEditSapceModule::onDebug(ndEvent* event) {
}


// ================ Frame ================
void ndEditSapceModule::onStartFrame(ndEvent* event) {
    // event->print(module_name);
}

void ndEditSapceModule::onDrawWindowFrame(ndEvent *event) {
}

void ndEditSapceModule::onDraw(ndEvent *event) {
    // mat4 view = camera.getView();
    // mat4 proj = camera.getProj();
    // mat4 pers_mat = proj*view; 

    // draw_routine.bindBuffer(R_Debug_DynamicBuffer2);
    // INT_ThickLine_compute_FrameData_T* frame_data = (INT_ThickLine_compute_FrameData_T*)draw_routine.writeBufferOpen();
    // frame_data[0].pers_mat = pers_mat;
    // frame_data[0].aspect_ratio = camera.getAspectRatio();
    // draw_routine.writeBufferClose();
}

void ndEditSapceModule::onEndFrame(ndEvent* event) {
    // event->print(module_name);
}


// ================ State Changes ================
void ndEditSapceModule::onResizeFrame(ndEvent *event) {
    vec4i v = event->getVec4i();
    camera.resetAspectRatio((float)v[0], (float)v[1]);
}


// ================ App Loop ================
void ndEditSapceModule::onBeginStartUp(ndEvent* event) {
    // // --- Initialize Draw Routine ---
    // draw_routine.bindBuffer(R_Debug_DynamicBuffer0);
    // draw_routine.createBuffer(DynamicBufferNew_T, sizeof(ThickLine_Point_T), 2);
    // draw_routine.bindBuffer(R_Debug_DynamicBuffer1);
    // draw_routine.createBuffer(DynamicBufferNew_T, sizeof(INT_ThickLine_Point_T), 18);
    // draw_routine.bindBuffer(R_Debug_DynamicBuffer2);
    // draw_routine.createBuffer(RapidBuffer_T, sizeof(INT_ThickLine_compute_FrameData_T), 1);

    // // --- Setting Buffer Data ---
    // draw_routine.bindBuffer(R_Debug_DynamicBuffer0);
    // ThickLine_Point_T* vertices = (ThickLine_Point_T*)draw_routine.writeBufferOpen();
    // vertices[0].position = { -0.5f, 0.0f, 0.0f, 1.0f };
    // vertices[1].position = {  0.5f, 0.0f, 0.0f, 1.0f };
    // vertices[0].width = 0.01f;

    // vertices[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    // vertices[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    // draw_routine.writeBufferModify(0, sizeof(ThickLine_Point_T)*2);

    // camera.setPosition({ 0.0f, 0.0f, 2.0f, 0.0f });

    // --- Testing Select Buffer ---
    points[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
    points[1].position = {  0.5f, -0.5f, 0.0f, 1.0f };
    points[2].position = { -0.5f,  0.5f, 0.0f, 1.0f };
    points[3].position = {  0.5f,  0.5f, 0.0f, 1.0f };

    test_mesh.createQuad({0, 1, 2, 3});
}

void ndEditSapceModule::onEndStartUp(ndEvent* event) {
    event->print(module_name);
}
