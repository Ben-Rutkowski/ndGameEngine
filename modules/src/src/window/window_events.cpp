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

void loadVertices(ndBuffer buffer) {
    StaticShape_VertexType* vertices = (StaticShape_VertexType*)buffer.editTap();
 
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

    buffer.editUntap();
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

void ndWindowModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);

    unsigned int debug_routine = nd_window.createDrawRoutine(ndDrawRoutineKindDebug);
    nd_window.bindRoutine(debug_routine);
    nd_window.bindBuffer(0);
    nd_window.createBuffer(6);

    ndBuffer buffer = nd_window.getBuffer();
    loadVertices(buffer);
    // buffer.debug();

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
    // ndBuffer buffer = nd_window.getBuffer();
    // StaticShape_VertexType* vertices = (StaticShape_VertexType*)buffer.editTap();

    // vertices[0].position = { -0.4f, -0.4f };

    // buffer.editUntap();
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
