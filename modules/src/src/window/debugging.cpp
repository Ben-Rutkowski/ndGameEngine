#include "camera.hpp"
#include "cocoa_interface.hpp"
#include "common.hpp"
#include "draw_routine_indices.h"
#include "window.hpp"
#define __INTERNAL__
#include "math/matrix.hpp"
#include "shader_types/S_triangle_types.h"
#include "shader_types/uniform_types.h"

#define DEBUG_BREAK 1
#define DEBUG_KILL  1
// static int DEBUG_count;

static Camera camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
static ndRoutine routine = { nullptr, 0 };

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    // --- Create Routine ---
    routine = nd_window.createDrawRoutine(DrawRoutineTriangle);
    routine.bindBuffer(R_Triangle_Vertices);
    routine.createBuffer(DynamicBuffer_T, sizeof(TrianglePoint_T), 6);
    routine.bindBuffer(R_Triangle_FrameData);
    routine.createBuffer(RapidBuffer_T, sizeof(UN_FrameDataNew_T), 1);

    // --- Add Buffer Data --- 
    routine.bindBuffer(R_Triangle_Vertices);
    TrianglePoint_T* vertices = (TrianglePoint_T*)routine.writeBufferOpen();
    vertices[0].position = { -0.5f, -0.5f, 0.0f, 1.0f, }; 
    vertices[1].position = {  0.5f, -0.5f, -1.0f, 1.0f, }; 
    vertices[2].position = {  0.5f,  0.5f, -1.0f, 1.0f, }; 

    vertices[3].position = {  0.5f,  0.5f, -1.0f, 1.0f, }; 
    vertices[4].position = { -0.5f,  0.5f, 0.0f, 1.0f, }; 
    vertices[5].position = { -0.5f, -0.5f, 0.0f, 1.0f, }; 

    vertices[0].color = { 0.3f, 0.3f, 0.3f, 1.0f };
    vertices[1].color = { 0.3f, 0.3f, 0.5f, 1.0f };
    vertices[2].color = { 0.3f, 0.3f, 0.5f, 1.0f };

    vertices[3].color = { 0.3f, 0.3f, 0.5f, 1.0f };
    vertices[4].color = { 0.3f, 0.3f, 0.3f, 1.0f };
    vertices[5].color = { 0.3f, 0.3f, 0.3f, 1.0f };
    routine.writeBufferClose();

    // --- Creating Camera ---
    camera = Camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
    camera.setPosition({ 0.0f, 0.0f, 2.0f, 0.0f, });
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    nd_window.armRoutine(routine);
    pollEventsCocoa();
}

void ndWindowModule::onDraw(ndEvent* event) {
    mat4 pers_mat = camera.getProj(); 

    routine.bindBuffer(R_Triangle_FrameData);
    UN_FrameDataNew_T* frame_data = (UN_FrameDataNew_T*)routine.writeBufferOpen();
    frame_data[0].pers_mat = pers_mat;
    routine.writeBufferClose();

    nd_window.drawView();
}

void ndWindowModule::onResizeFrame(ndEvent* event) {
    vec4i v = event->getVec4i();
    camera.resetAspectRatio((float)v[0], (float)v[1]);
}

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);

    // DEBUG_count += 1;
    // if (DEBUG_count == DEBUG_BREAK) 
    // {

    // } 
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}
