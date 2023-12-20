#include "application.hpp"

void ndApp::startApp() {
    beginLoop();
    pollEvents();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        startFrame();
        pollEvents();

        drawWindowFrame();
        draw();
        pollEvents();
        
        endFrame();
        pollEvents();
    }
}