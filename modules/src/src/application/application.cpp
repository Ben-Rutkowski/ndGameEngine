#include "application.hpp"

ndApp::ndApp()
    :ndModule<ndAppStateLen>(Module::APPLICATION),
    window{ nullptr } {

    event_manager.linkCallback(this, ndApp::propogateEventCallback);
}

void ndApp::linkWindow(ndModule* window_ptr) {
    throw std::runtime_error("Not Implemented ndApp::linkWindow");
}