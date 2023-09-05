#include "application.hpp"

// === Events ===
void ndApp::propogateEvent(Event* event) { 
    window->runEvent(event);
}

void ndApp::pollEvents() {
    event_manager.pollEvents();
}

void ndApp::beginLoop() {
    Event event(module_name, Data::BEGIN_LOOP);
    propogateEvent(&event);
}

void ndApp::startFrame() {
    Event event(module_name, Data::START_FRAME);
    propogateEvent(&event);
}

void ndApp::drawFrame() {
    Event event(module_name, Data::DRAW_FRAME);
    propogateEvent(&event);
}

void ndApp::endFrame() {
    Event event(module_name, Data::END_FRAME);
    propogateEvent(&event);
}