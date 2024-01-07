#include "window.hpp"

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    float x;
    float y;
} vec2;
#define __ND_VECTOR__
#include "shader_types.h"


void ndWindowModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::DEBUG:             onDebug(event); break;
    case Operation::BEGIN_START_UP:    onBeginStartUp(event); break;
    case Operation::END_START_UP:      onEndStartUp(event); break;
    case Operation::START_FRAME:       onStartFrame(event); break;
    case Operation::DRAW_WINDOW_FRAME: onDrawWindowFrame(event); break;
    case Operation::DRAW:              onDraw(event); break;
    case Operation::END_FRAME:         onEndFrame(event); break;
    case Operation::CLOSE_APP:         onCloseApp(event); break;
    default: break;
    }
}

static ndRoutine line_routine   = ndRoutine(nullptr, 0);

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);

    line_routine = nd_window.createDrawRoutine(ndDrawRoutineKindLine);
    line_routine.bindBuffer(Line_B_Vertices);
    line_routine.createPublicBuffer(sizeof(Line_TriagVtype), 3);

    line_routine.bindBuffer(Line_B_Vertices);
    Line_TriagVtype* vertices = (Line_TriagVtype*)line_routine.writeBufferOpen();
    vertices[0].position = { -0.5f, -0.5f }; 
    vertices[1].position = {  0.5f, -0.5f };
    vertices[2].position = {  0.0f,  0.5f };

    vertices[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
    vertices[1].color = { 0.0f, 1.0f, 0.0f, 1.0f };
    vertices[2].color = { 0.0f, 0.0f, 1.0f, 1.0f };
    line_routine.writeBufferClose();
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    event->print(module_name);
    nd_window.bindRoutine(line_routine);
    nd_window.armRoutine();
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);
    line_routine.writeBufferOpen();
    line_routine.writeBufferClose();
}


// ================ Frame ================
void ndWindowModule::onStartFrame(ndEvent* event) {
}

void ndWindowModule::onDrawWindowFrame(ndEvent* event) {
}

void ndWindowModule::onDraw(ndEvent* event) {
    nd_window.drawView();
}

void ndWindowModule::onEndFrame(ndEvent* event) {
}

void ndWindowModule::onCloseApp(ndEvent* event) {
    event->print(module_name);
}
