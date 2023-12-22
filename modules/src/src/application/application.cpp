#include "application.hpp"
#include "cocoa_interface.hpp"

ndAppModule::ndAppModule()
    :ndModule<ndAppStateLen>(Module::APPLICATION),
    window{ nullptr } {

    event_manager.linkCallback(this, ndAppModule::propogateEventCallback);

    initCocoa();
}

void ndAppModule::linkWindow(ndModule* window_ptr) {
    window = window_ptr;
    window->setManagerPtr(&event_manager);
}