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

void loadVertices(StaticShape_VertexType* vertices) {
    vertices[0].position = { -0.5f, -0.5f };
    vertices[1].position = {  0.5f, -0.5f };
    vertices[2].position = { -0.5f,  0.5f };

    vertices[3].position = { -0.5f,  0.5f };
    vertices[4].position = {  0.5f, -0.5f };
    vertices[5].position = {  0.5f,  0.5f };

    vertices[0].color = { 0.0f, 0.5f, 0.7f, 1.0f };
    vertices[1].color = { 0.3f, 0.5f, 0.7f, 1.0f };
    vertices[2].color = { 0.5f, 0.5f, 0.7f, 1.0f };

    vertices[3].color = { 0.5f, 0.5f, 0.7f, 1.0f };
    vertices[4].color = { 0.3f, 0.5f, 0.7f, 1.0f };
    vertices[5].color = { 0.5f, 0.9f, 0.7f, 1.0f };
}

void loadVertices(Line_Triangalized_VertexType* vertices) {
    const float x = 0.5f;
    const float y = 0.05f;
    const float z = 0.0f;
    const float o = 1.0f;
    int tri = 0;

    tri = 0;
    vertices[3*tri+0].position = { -x, -y };
    vertices[3*tri+1].position = {  x, -y };
    vertices[3*tri+2].position = { -x,  y };

    vertices[3*tri+0].uv = { z, z };
    vertices[3*tri+1].uv = { z, z };
    vertices[3*tri+2].uv = { z, z };

    tri = 1;
    vertices[3*tri+0].position = {  x,  y };
    vertices[3*tri+1].position = { -x,  y };
    vertices[3*tri+2].position = {  x, -y };

    vertices[3*tri+0].uv = { z, z };
    vertices[3*tri+1].uv = { z, z };
    vertices[3*tri+2].uv = { z, z };

    tri = 2;
    vertices[3*tri+0].position = { x,    y };
    vertices[3*tri+1].position = { x+y,  y };
    vertices[3*tri+2].position = { x,   -y };

    vertices[3*tri+0].uv = { -o, z };
    vertices[3*tri+1].uv = { -o, o };
    vertices[3*tri+2].uv = {  o, z };

    tri = 3;
    vertices[3*tri+0].position = { x+y, -y };
    vertices[3*tri+1].position = { x,   -y };
    vertices[3*tri+2].position = { x+y,  y };

    vertices[3*tri+0].uv = {  o, o };
    vertices[3*tri+1].uv = {  o, z };
    vertices[3*tri+2].uv = { -o, o };

    tri = 4;
    vertices[3*tri+0].position = { -x,   -y };
    vertices[3*tri+1].position = { -x-y, -y };
    vertices[3*tri+2].position = { -x,    y };

    vertices[3*tri+0].uv = { -o, z };
    vertices[3*tri+1].uv = { -o, o };
    vertices[3*tri+2].uv = {  o, z };

    tri = 5;
    vertices[3*tri+0].position = { -x-y,  y };
    vertices[3*tri+1].position = { -x,    y };
    vertices[3*tri+2].position = { -x-y, -y };

    vertices[3*tri+0].uv = {  o, o };
    vertices[3*tri+1].uv = {  o, z };
    vertices[3*tri+2].uv = { -o, o };

    for (int i=0; i<18; i++) {
        vertices[i].color = {1.0f, 1.0f, 1.0f, 1.0f};
    }
}

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

static ndRoutine static_routine = ndRoutine(nullptr, 0);
static ndRoutine line_routine   = ndRoutine(nullptr, 0);

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);

// --- Static Routine ---
    static_routine = nd_window.createDrawRoutine(ndDrawRoutineKindDebug);
    nd_window.bindRoutine(static_routine);
    static_routine.bindBuffer(0);
    static_routine.createBuffer(6);
    StaticShape_VertexType* vertices = (StaticShape_VertexType*)static_routine.writeBufferOpen();
    loadVertices(vertices);
    static_routine.writeBufferClose();
    nd_window.configureRoutine();
// --- Static Routine ---

// --- Line Routine ---
    line_routine = nd_window.createDrawRoutine(ndDrawRoutineKindLine);
    nd_window.bindRoutine(line_routine);
    line_routine.bindBuffer(0);
    line_routine.createBuffer(18);
    Line_Triangalized_VertexType* verts = (Line_Triangalized_VertexType*)line_routine.writeBufferOpen();
    loadVertices(verts);
    line_routine.writeBufferClose();
    nd_window.configureRoutine();
// --- Line Routine ---
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    event->print(module_name);
    // nd_window.bindRoutine(static_routine);
    nd_window.bindRoutine(line_routine);
    nd_window.armRoutine();
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);
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
