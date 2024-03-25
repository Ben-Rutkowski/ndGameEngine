#include "camera.hpp"
#include "cocoa_interface.hpp"
#include "draw_routine_indices.h"
#include "shader_types/S_triangle_types.h"
#include "window.hpp"
#define __INTERNAL__
#include "shader_types/uniform_types.h"
#include "shader_types/S_thick_line_types.h"
#define DEBUG_BREAK 1
#define DEBUG_KILL  1

static int DEBUG_count;

static ndRoutine debug_routine    = {nullptr, 0};
static ndRoutine triangle_routine = {nullptr, 0};

static Camera camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    // --- Create Draw Routine ---
    debug_routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    // --- Create Buffers ---
    debug_routine.bindBuffer(R_Debug_DynamicBuffer1);
    debug_routine.createBuffer(DynamicBuffer_T, sizeof(INT_Point_T), 18*2);
    debug_routine.bindBuffer(R_Debug_DynamicBuffer0);
    debug_routine.createBuffer(DynamicBuffer_T, sizeof(ThickLinePoint_T), 4);
    debug_routine.bindBuffer(R_Debug_DynamicBuffer2);
    debug_routine.createBuffer(RapidBuffer_T, sizeof(UN_FrameData_T), 1);
    // --- Writing Buffer ---
    debug_routine.bindBuffer(R_Debug_DynamicBuffer0);
    ThickLinePoint_T* vertices = (ThickLinePoint_T*)debug_routine.writeBufferOpen();
    vertices[0].position = { -0.5f, 0.0f };
    vertices[1].position = {  0.5f, 0.0f };

    vertices[0].color = { 0.3f, 0.3f, 0.0f, 1.0f};
    vertices[1].color = { 0.0f, 0.0f, 0.0f, 1.0f};

    vertices[2].position = { -0.5f, 0.0f };
    vertices[3].position = {  0.5f, 0.5f };

    vertices[2].color = { 0.3f, 0.3f, 0.0f, 1.0f};
    vertices[3].color = { 0.0f, 0.0f, 0.0f, 1.0f};
    debug_routine.writeBufferClose();

    // --- Create Draw Routine ---
    triangle_routine = nd_window.createDrawRoutine(DrawRoutineTriangle);
    // --- Create Buffers ---
    triangle_routine.bindBuffer(R_Triangle_Vertices);
    triangle_routine.createBuffer(DynamicBuffer_T, sizeof(TrianglePoint_T), 6);
    triangle_routine.bindBuffer(R_Triangle_FrameData);
    triangle_routine.createBuffer(RapidBuffer_T, sizeof(UN_FrameDataNew_T), 1);
    // --- Writing Buffer ---
    triangle_routine.bindBuffer(R_Triangle_Vertices);
    TrianglePoint_T* tri_verts = (TrianglePoint_T*)triangle_routine.writeBufferOpen();
    tri_verts[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
    tri_verts[1].position = {  0.5f, -0.5f, 0.0f, 1.0f };
    tri_verts[2].position = {  0.5f,  0.5f, 0.0f, 1.0f };

    tri_verts[3].position = {  0.5f,  0.5f, 0.0f, 1.0f };
    tri_verts[4].position = { -0.5f,  0.5f, 0.0f, 1.0f };
    tri_verts[5].position = { -0.5f, -0.5f, 0.0f, 1.0f };

    tri_verts[0].color = { 0.5f, 0.5f, 0.5f, 0.5f };
    tri_verts[1].color = { 0.5f, 0.5f, 0.5f, 0.5f };
    tri_verts[2].color = { 0.5f, 0.5f, 0.5f, 0.5f };

    tri_verts[3].color = { 0.5f, 0.5f, 0.5f, 0.5f };
    tri_verts[4].color = { 0.5f, 0.5f, 0.5f, 0.5f };
    tri_verts[5].color = { 0.5f, 0.5f, 0.5f, 0.5f };
    triangle_routine.writeBufferClose();

    camera = Camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
    camera.setPosition({0.0f, 0.0f, 2.0f, 1.0f});
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    // nd_window.armRoutine(debug_routine);
    nd_window.armRoutine(triangle_routine);
    pollEventsCocoa();
}

void ndWindowModule::onDraw(ndEvent* event) {
    // debug_routine.bindBuffer(R_Debug_DynamicBuffer2);
    // ScreenSize size = nd_window.getScreenSize();

    // UN_FrameData_T* frame_data = (UN_FrameData_T*)debug_routine.writeBufferOpen();
    // frame_data[0].aspect_ratio = (float)(size.width/size.height);
    // frame_data[0].thickness    = 0.01f;
    // debug_routine.writeBufferClose();

    triangle_routine.bindBuffer(R_Triangle_FrameData);
    UN_FrameDataNew_T* frame_data = (UN_FrameDataNew_T*)triangle_routine.writeBufferOpen();

    mat4 pers_mat = camera.getProj();
    frame_data[0].pers_mat = pers_mat;

    triangle_routine.writeBufferClose();
    nd_window.drawView();
}

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {

    } 
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}
