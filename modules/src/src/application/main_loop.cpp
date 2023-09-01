#include "application.hpp"
#include "edit_mesh.hpp"

void ndApp::startApp() {
    beginLoop();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    while (!window->shouldClose()) {
        startFrame();
        pollEvents();

        drawFrame();
        pollEvents();
        
        endFrame();
        pollEvents();
    }
}