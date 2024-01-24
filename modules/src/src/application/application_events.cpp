#include "application.hpp"

void ndAppModule::propogateEvent(ndEvent* event) {
    window->runEvent(event);
}

void ndAppModule::pollEvents() {
    event_manager.pollEvents();
}

void ndAppModule::beginStartUp() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::BEGIN_START_UP)
    );
}

void ndAppModule::endStartUp() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::END_START_UP)
    );
}

void ndAppModule::startFrame() {
//     float delta = window->requestFloat(Request::FRAME_DELTA);
//     vec4  mouse = window->requestVec4(Request::MOUSE_POSITION);

    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::START_FRAME)
    );
}

void ndAppModule::drawWindowFrame() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::DRAW_WINDOW_FRAME)
    );
}

void ndAppModule::draw() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::DRAW)
    );
}

void ndAppModule::endFrame() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::END_FRAME)
    );
}

// === Static ===
void ndAppModule::propogateEventCallback(void* app_void_ptr, ndEvent* event) {
    ndAppModule* app_ptr = (ndAppModule*)app_void_ptr;
    app_ptr->propogateEvent(event);
}
