#include "application.hpp"

// === Events ===
void ndApp::propogateEvent(Event* event) { 
    window->runEvent(event);
}

void ndApp::pollEvents() {
    event_manager.pollEvents();
}

void ndApp::beginLoop() {
    event_manager.propogateEvent(module_name, Data::BEGIN_LOOP);
}

void ndApp::startFrame() {
    float delta = window->requestFloat(Request::FRAME_DELTA);
    vec4  mouse = window->requestVec4(Request::MOUSE_POSITION);
    mouse.set(delta, 2);
    event_manager.propogateEvent4f(module_name, Data::START_FRAME, mouse);
}

void ndApp::drawFrame() {
    event_manager.propogateEvent(module_name, Data::DRAW_FRAME);
}

void ndApp::endFrame() {
    event_manager.propogateEvent(module_name, Data::END_FRAME);
}

// === Static ===
void ndApp::propogateEventCallback(void* app_ptr, Event* event) {
    ndApp* app = (ndApp*)app_ptr;
    app->propogateEvent(event);
}