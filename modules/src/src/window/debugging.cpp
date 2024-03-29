#include "camera.hpp"
#include "cocoa_interface.hpp"
#include "draw_routine_indices.h"
#include "window.hpp"
#define __INTERNAL__
#include "math/matrix.hpp"
#include "shader_types/S_thick_line_types.h"
#include "shader_types/uniform_types.h"

#define DEBUG_BREAK 5
#define DEBUG_KILL  1
static int DEBUG_count;

static Camera camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
static ndRoutine routine = { nullptr, 0 };

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    // --- Create Routine ---
    routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    routine.bindBuffer(R_Debug_DynamicBuffer0);
    routine.createBuffer(DynamicBuffer_T, sizeof(ThickLine_Point_T), 2);
    routine.bindBuffer(R_Debug_DynamicBuffer1);
    routine.createBuffer(DynamicBuffer_T, sizeof(INT_ThickLine_Point_T), 18);
    routine.bindBuffer(R_Debug_DynamicBuffer2);
    routine.createBuffer(RapidBuffer_T, sizeof(INT_ThickLine_compute_FrameData_T), 1);
    // --- Setting Buffer Data ---
    routine.bindBuffer(R_Debug_DynamicBuffer0);
    ThickLine_Point_T* vertices = (ThickLine_Point_T*)routine.writeBufferOpen();
    vertices[0].position = { -0.5f, 0.0f, 0.0f, 1.0f };
    vertices[1].position = {  0.5f, 0.6f, 0.0f, 1.0f };

    vertices[0].width = 0.05f;

    vertices[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    vertices[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    routine.writeBufferClose();

    // --- Creating Camera ---
    camera = Camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
    camera.setPosition({ 0.0f, 0.0f, 2.0f, 0.0f });
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    nd_window.armRoutine(routine);
    pollEventsCocoa();
}

void ndWindowModule::onDraw(ndEvent* event) {
    mat4 view = camera.getView();
    mat4 proj = camera.getProj();

    routine.bindBuffer(R_Debug_DynamicBuffer2);
    INT_ThickLine_compute_FrameData_T* frame_data = (INT_ThickLine_compute_FrameData_T*)routine.writeBufferOpen();
    frame_data[0].view = view;
    frame_data[0].pers = proj;
    routine.writeBufferClose();
    
    nd_window.drawView();

    // --- Debugging ---
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {
        // routine.bindBuffer(R_Debug_DynamicBuffer1);
        // routine.debug(6);
    } 

    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}

void ndWindowModule::onResizeFrame(ndEvent* event) {
    vec4i v = event->getVec4i();
    camera.resetAspectRatio((float)v[0], (float)v[1]);
}

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);
}
