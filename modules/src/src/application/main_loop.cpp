#include "application.hpp"
#include "edit_mesh.hpp"

void ndApp::startApp() {
    beginLoop();
    pollEvents();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        startFrame();
        pollEvents();

        drawFrame();
        pollEvents();
        
        endFrame();
        pollEvents();
    }
}