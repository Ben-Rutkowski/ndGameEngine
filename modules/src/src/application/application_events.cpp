#include "application.hpp"

void ndApp::propogateEvent(ndEvent* event) {
    // window->runEvent(event);

    event->print(module_name);
}

void ndApp::pollEvents() {
    event_manager.pollEvents();
}

void ndApp::beginLoop() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::BEGIN_LOOP)
    );
}

void ndApp::startFrame() {
    // float delta = window->requestFloat(Request::FRAME_DELTA);
    // vec4  mouse = window->requestVec4(Request::MOUSE_POSITION);

    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::START_FRAME)
    );
}

void ndApp::drawWindowFrame() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::DRAW_WINDOW_FRAME)
    );
}

void ndApp::draw() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::DRAW)
    );
}

void ndApp::endFrame() {
    event_manager.propogateEventImmediate(
        ndEvent(module_name, Operation::END_FRAME)
    );
}

// === Static ===
void ndApp::propogateEventCallback(void* app_void_ptr, ndEvent* event) {
    ndApp* app_ptr = (ndApp*)app_void_ptr;
    app_ptr->propogateEvent(event);
}