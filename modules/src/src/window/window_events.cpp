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

    // vertices[0].color = { 0.0f, 0.5f, 0.7f, 1.0f };
    vertices[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
    vertices[1].color = { 0.3f, 0.5f, 0.7f, 1.0f };
    vertices[2].color = { 0.5f, 0.5f, 0.7f, 1.0f };

    vertices[3].color = { 0.5f, 0.5f, 0.7f, 1.0f };
    vertices[4].color = { 0.3f, 0.5f, 0.7f, 1.0f };
    // vertices[5].color = { 0.5f, 0.9f, 0.7f, 1.0f };
    vertices[5].color = { 0.0f, 0.0f, 0.0f, 0.0f };
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

static ndRoutine routine = ndRoutine(nullptr, 0);

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);

    routine = nd_window.createDrawRoutine(ndDrawRoutineKindDebug);
    nd_window.bindRoutine(routine);
    routine.bindBuffer(0);
    routine.createBuffer(6);
    StaticShape_VertexType* vertices = (StaticShape_VertexType*)routine.writeBufferOpen();
    loadVertices(vertices);
    routine.writeBufferClose();

    nd_window.configureRoutine();
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    event->print(module_name);
    nd_window.armRoutine();
    nd_window.showWindow();
    pollEventsCocoa();
}

void ndWindowModule::onDebug(ndEvent* event) {
    event->print(module_name);
    // routine.writeBufferOpen();
    // routine.writeBufferClose();
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
