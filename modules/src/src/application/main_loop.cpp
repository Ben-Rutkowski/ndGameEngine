#include "application.hpp"

void ndApp::startApp() {
    beginLoop();
    while (!window->shouldClose()) {
        startFrame();

        pollEvents();
        endFrame();
    }
}