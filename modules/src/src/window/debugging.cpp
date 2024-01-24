#include "window.hpp"
#include "math/matrix.hpp"
#include "shader_types.h"
#define DEBUG_BREAK 5
#define DEBUG_KILL  5
static int DEBUG_count;

static ndRoutine debug_routine = {nullptr, 0};

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    DEBUG_count = 0;
    debug_routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    debug_routine.bindBuffer(R_Debug_Buffer1);
    debug_routine.createAuxBuffer(sizeof(Line_TriagVtype), 8*18);
    debug_routine.bindBuffer(R_Debug_Buffer0);
    debug_routine.createPublicBuffer(sizeof(Line_PtVtype), 8);
    Line_PtVtype* vertices = (Line_PtVtype*)debug_routine.writeBufferOpen();
    vertices[0].position = { -0.5f, 0.4f };
    vertices[1].position = {  0.5f, 0.5f };
    vertices[2].position = { -0.5f, 0.1f };
    vertices[3].position = {  0.5f, 0.7f };

    vertices[4].position = { -0.5f, -0.9f };
    vertices[5].position = {  0.5f, -0.7f };
    vertices[6].position = { -0.5f, -0.3f };
    vertices[7].position = {  0.5f, -0.1f };

    for (int i=0; i<8; i++) {
        vertices[i].color = { 0.0f, 0.3f, 0.5f, 1.0f };
    }

    debug_routine.writeBufferClose();
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    nd_window.armRoutine(debug_routine);
    pollEventsCocoa();
}

typedef struct {
    alignas(16) vec2 position;
    alignas(16) vec4 color;
} TestStruct;

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {
        
    } 
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}