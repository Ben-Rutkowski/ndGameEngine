#include "window.hpp"
typedef struct { float x; float y; float z; float w; } vec4;
typedef struct { float x; float y; } vec2;
#define __ND_VECTOR__
#include "shader_types.h"
#define DEBUG_BREAK 5
#define DEBUG_KILL  5
static int DEBUG_count;


// ======== ========
static ndRoutine line_routine = ndRoutine(nullptr, 0);

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    DEBUG_count = 0;

    line_routine = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    line_routine.bindBuffer(R_Line_Vertices);
    line_routine.createPublicBuffer(sizeof(Line_TriagVtype), 3);
    line_routine.bindBuffer(R_Line_LinePoints);
    line_routine.createPublicBuffer(sizeof(Line_PtVtype), 4);

    Line_PtVtype* vertices = (Line_PtVtype*)line_routine.writeBufferOpen();
    vertices[0].position = { -0.5f, 0.0f };
    vertices[1].position = {  0.5f, 0.3f };
    vertices[0].color    = { 0.05f, 0.05f, 0.05f, 1.0f };
    vertices[1].color    = { 0.45f, 0.3f, 0.1f, 1.0f };

    vertices[2].position = { -0.5f, 0.3f };
    vertices[3].position = {  0.5f, 0.3f };
    vertices[2].color    = { 0.05f, 0.05f, 0.05f, 1.0f };
    vertices[3].color    = { 0.45f, 0.3f, 0.1f, 1.0f };
    line_routine.writeBufferClose();

    line_routine.bindBuffer(R_Line_TriangRelay);
    line_routine.createPublicBuffer(sizeof(Line_TriagVtype), 36);
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.armRoutine(line_routine);
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDebug(ndEvent* event) {
    DEBUG_count += 1;
    if (DEBUG_count == DEBUG_BREAK) 
    {
        // line_routine.debug(1);
    }
    
    // if (DEBUG_count == DEBUG_KILL) {
    //     killCocoa();
    // }
}