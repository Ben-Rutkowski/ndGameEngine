#include <iostream>
#include "application.hpp"

// === Constructors and Initialization ===
Module ndApp::module_name = Module::APPLICATION;

ndApp::ndApp() 
    :window{ nullptr } { 
    // EventCallOld event_call(PACK(ndApp::propogateEvent));
    // event_manager.setCallback(event_call);

    event_manager.setCallback(this, ndApp::propogateEventCallback);
}

// void ndApp::linkWindow(ndModuleOld* window_ptr) {
//     window = window_ptr;
//     window_ptr->setManagerPtr(&event_manager);
// }

void ndApp::linkWindow(ndModule* window_ptr) {
    window = window_ptr;
    window_ptr->setManagerPtr(&event_manager);
}