#include "application.hpp"

// === Events ===
void ndApp::propogateEvent(Event* event) { 
    window->runEventWindow(event);
}

// // === Private ===
// void ndApp::pollEvents() { event_manager.pollEvents(); }