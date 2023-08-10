#include "application.hpp"

// === Events ===
void ndApp::propogateEvent(Event* event) { 
    window->runEventWindow(event);
}

void ndApp::startFrame() {
    const EventCode code(EventType::ACTION, module_name, Data::START_FRAME);
    Event event(code);
    propogateEvent(&event);
}

// // === Private ===
// void ndApp::pollEvents() { event_manager.pollEvents(); }