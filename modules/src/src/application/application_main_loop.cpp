#include "application.hpp"
#include "cocoa_interface.hpp"

void ndAppModule::startApp() {
    beginLoop();
    pollEvents();

    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        startFrame();
        drawWindowFrame();
        draw();
        endFrame();

        pollEvents();
        pollEventsCocoa();
    }
}