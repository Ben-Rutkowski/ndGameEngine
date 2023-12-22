#include "application.hpp"

void ndApp::startApp() {
    beginLoop();
    pollEvents();

    // // while (!window->requestBool(Request::SHOULD_CLOSE)) {
    while (true) {
        startFrame();
        drawWindowFrame();
        draw();
        endFrame();

        pollEvents();
    }
}