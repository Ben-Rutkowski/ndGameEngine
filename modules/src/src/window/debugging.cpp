#include "draw_routine_indices.h"
#include "window.hpp"
#include "math/matrix.hpp"
#define __INTERNAL__
#include "shader_types/S_thick_line_types.h"
#define DEBUG_BREAK 5
#define DEBUG_KILL  5
static int DEBUG_count;

static ndRoutine debug_routine = {nullptr, 0};

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    debug_routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    debug_routine.bindBuffer(R_Debug_DynamicBuffer1);
    debug_routine.createAuxBuffer(sizeof(INT_Point_T), 18*2);
    debug_routine.bindBuffer(R_Debug_DynamicBuffer0);
    debug_routine.createPublicBuffer(sizeof(ThickLinePoint_T), 4);
    ThickLinePoint_T* vertices = (ThickLinePoint_T*)debug_routine.writeBufferOpen();
    vertices[0].position = { -0.5f, 0.0f };
    vertices[1].position = {  0.5f, 0.0f };

    vertices[0].color = { 1.0f, 1.0f, 1.0f, 1.0f};
    vertices[1].color = { 1.0f, 1.0f, 1.0f, 1.0f};

    vertices[2].position = { -0.5f, 0.0f };
    vertices[3].position = {  0.5f, 0.5f };

    vertices[2].color = { 1.0f, 0.3f, 0.0f, 1.0f};
    vertices[3].color = { 1.0f, 1.0f, 1.0f, 1.0f};
    debug_routine.writeBufferClose();
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    nd_window.armRoutine(debug_routine);
    pollEventsCocoa();
}

void ndWindowModule::onDraw(ndEvent* event) {
    nd_window.drawView();
}

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {
        // debug_routine.bindBuffer(R_Debug_Buffer1);
        // debug_routine.debug(6);
    } 
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}
