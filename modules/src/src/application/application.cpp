#include "application.hpp"
#include "cocoa_interface.hpp"

ndAppModule::ndAppModule()
    :ndModuleImplement<ndAppStateLen>(Module::APPLICATION),
    window{ nullptr } {

    event_manager.linkCallback(this, ndAppModule::propogateEventCallback);

    setEventManagerCocoa(&event_manager);
    setEventCallbackCocoa(event_manager.cocoaCallbackAnchor);
    setEventCallbackCocoaVI2(event_manager.cocoaCallbackVI2Anchor);

    initCocoa();
}

void ndAppModule::linkWindow(ndModule* window_ptr) {
    window = window_ptr;
    window->setManagerPtr(&event_manager);
    state_cache.set(ndAppStateConfigured, true);
}
