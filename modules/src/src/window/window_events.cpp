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

void loadVertices(void* buffer) {
    StaticShape_VertexType* vertices = (StaticShape_VertexType*)buffer;
 
    vertices[0].position = { -0.5f, -0.5f };
    vertices[1].position = {  0.5f, -0.5f };
    vertices[2].position = {  0.0f,  0.5f };

    vertices[0].color = { 0.0f, 0.5f, 0.7f, 1.0f };
    vertices[1].color = { 0.3f, 0.5f, 0.7f, 1.0f };
    vertices[2].color = { 0.5f, 0.5f, 0.7f, 1.0f };
}

void ndWindowModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::BEGIN_LOOP:        onBeginLoop(event); break;
    case Operation::START_FRAME:       onStartFrame(event); break;
    case Operation::DRAW_WINDOW_FRAME: onDrawWindowFrame(event); break;
    case Operation::DRAW:              onDraw(event); break;
    case Operation::END_FRAME:         onEndFrame(event); break;
    case Operation::CLOSE_APP:         onCloseApp(event); break;
    default: break;
    }
}

void ndWindowModule::onBeginLoop(ndEvent* event) {
    pollEventsCocoa();

    unsigned int debug_routine = nd_window.createDrawRoutine(ndDrawRoutineKindDebug);
    nd_window.bindRoutine(debug_routine);
    nd_window.bindBuffer(0);
    nd_window.createBuffer(3);

    void* buffer = nd_window.getBuffer();
    loadVertices(buffer);

    nd_window.configureRoutine();

    nd_window.armRoutine();

    nd_window.showWindow();
    event->print(module_name);
}

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
