#include "application.hpp"
#include "cocoa_interface.hpp"
#include <chrono>

void ndAppModule::startApp() {
    beginStartUp();

    endStartUp();
    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        startFrame();
        drawWindowFrame();
        draw();
        endFrame();

        pollEvents();
        pollEventsCocoa();

        event_manager.propogateEventImmediate(
            ndEvent(module_name, Operation::DEBUG)
        );
    }
}
