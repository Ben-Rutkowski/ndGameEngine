#include "window.hpp"

#include "camera.hpp"
#include "cocoa_interface.hpp"
#include "draw_routine_indices.h"
#define __INTERNAL__
#include "math/matrix.hpp"
#include "shader_types/S_thick_line_types.h"

#define DEBUG_BREAK 5
#define DEBUG_KILL  1
// static int DEBUG_count;
// static float c;
// 
// static Camera camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
// static ndRoutine routine = { nullptr, 0 };

// void ndWindowModule::onBeginStartUp(ndEvent* event) {
//     // --- Create Routine ---
//     routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
//     routine.bindBuffer(R_Debug_DynamicBuffer0);
//     routine.createBuffer(DynamicBufferNew_T, sizeof(ThickLine_Point_T), 2*8);
//     routine.bindBuffer(R_Debug_DynamicBuffer1);
//     routine.createBuffer(DynamicBufferNew_T, sizeof(INT_ThickLine_Point_T), 18*8);
//     routine.bindBuffer(R_Debug_DynamicBuffer2);
//     routine.createBuffer(RapidBuffer_T, sizeof(INT_ThickLine_compute_FrameData_T), 1);
//     
//     // --- Setting Buffer Data ---
//     routine.bindBuffer(R_Debug_DynamicBuffer0);
//     ThickLine_Point_T* vertices = (ThickLine_Point_T*)routine.writeBufferOpen();
// 
//     // --- FRONT ---
//     vertices[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
//     vertices[1].position = {  0.5f, -0.5f, 0.0f, 1.0f };
//     vertices[0].width = 0.01f;
// 
//     vertices[2].position = {  0.5f, -0.5f, 0.0f, 1.0f };
//     vertices[3].position = {  0.5f,  0.5f, 0.0f, 1.0f };
//     vertices[2].width = 0.01f;
// 
//     vertices[4].position = {  0.5f,  0.5f, 0.0f, 1.0f };
//     vertices[5].position = { -0.5f,  0.5f, 0.0f, 1.0f };
//     vertices[4].width = 0.01f;
// 
//     vertices[6].position = { -0.5f,  0.5f, 0.0f, 1.0f };
//     vertices[7].position = { -0.5f, -0.5f, 0.0f, 1.0f };
//     vertices[6].width = 0.01f;
// 
//     // --- BACK ---
//     vertices[8].position = { -0.5f, -0.5f, -1.0f, 1.0f };
//     vertices[9].position = {  0.5f, -0.5f, -1.0f, 1.0f };
//     vertices[8].width = 0.01f;
// 
//     vertices[10].position = {  0.5f, -0.5f, -1.0f, 1.0f };
//     vertices[11].position = {  0.5f,  0.5f, -1.0f, 1.0f };
//     vertices[10].width = 0.01f;
// 
//     vertices[12].position = {  0.5f,  0.5f, -1.0f, 1.0f };
//     vertices[13].position = { -0.5f,  0.5f, -1.0f, 1.0f };
//     vertices[12].width = 0.01f;
// 
//     vertices[14].position = { -0.5f,  0.5f, -1.0f, 1.0f };
//     vertices[15].position = { -0.5f, -0.5f, -1.0f, 1.0f };
//     vertices[14].width = 0.01f;
// 
//     vertices[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[2].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[3].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[4].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[5].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[6].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[7].color = { 0.0f, 0.0f, 0.0f, 1.0f };
// 
//     vertices[8].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[9].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[10].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[11].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[12].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[13].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[14].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     vertices[15].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//     routine.writeBufferModify(0, sizeof(ThickLine_Point_T)*2);
// 
//     // --- Creating Camera ---
//     camera = Camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f);
//     camera.setPosition({ 0.0f, 0.0f, 2.0f, 0.0f });
// 
//     c = 0.0f;
// }

// void ndWindowModule::onEndStartUp(ndEvent* event) {
//     nd_window.showWindow();
//     nd_window.armRoutine(routine);
//     pollEventsCocoa();
// }

// void ndWindowModule::onDraw(ndEvent* event) {
//     mat4 view = camera.getView();
//     mat4 proj = camera.getProj();
//     mat4 pers_mat = proj*view;
// 
//     routine.bindBuffer(R_Debug_DynamicBuffer2);
//     INT_ThickLine_compute_FrameData_T* frame_data = (INT_ThickLine_compute_FrameData_T*)routine.writeBufferOpen();
//     frame_data[0].pers_mat = pers_mat;
//     frame_data[0].aspect_ratio = camera.getAspectRatio();
//     routine.writeBufferClose();
// 
//     nd_window.drawView();
// 
//     // if (DEBUG_count == DEBUG_KILL) {
//     //     killCocoa();
//     // }
// }

// void ndWindowModule::onResizeFrame(ndEvent* event) {
//     vec4i v = event->getVec4i();
//     camera.resetAspectRatio((float)v[0], (float)v[1]);
// }

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);
}
