#include "application.hpp"

// === Events ===
void ndApp::propogateEvent(Event* event) { 
    window->runEventWindow(event);
}

void ndApp::pollEvents() {
    event_manager.pollEvents();
}

void ndApp::beginLoop() {
    CODE(begin_loop, Data::BEGIN_LOOP)
    Event event(begin_loop);
    propogateEvent(&event);
}

void ndApp::startFrame() {
    CODE(start_frame, Data::START_FRAME)
    Event event(start_frame);
    propogateEvent(&event);
}

void ndApp::endFrame() {
    CODE(end_frame, Data::END_FRAME)
    Event event(end_frame);
    propogateEvent(&event);
}