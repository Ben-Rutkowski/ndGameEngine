#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    event_interface.setCallback(Data::BEGIN_LOOP,  PACK(EditSpace::onBeginLoop));
    event_interface.setCallback(Data::START_FRAME, PACK(EditSpace::onStartFrame));
    event_interface.setCallback(Data::DRAW_FRAME,  PACK(EditSpace::onDrawFrame));
    event_interface.setCallback(Data::END_FRAME,   PACK(EditSpace::onEndFrame));
}

void EditSpace::runEventEditSpace(Event* event) {
    event_interface(event);
}

// === On Events ===
void EditSpace::onBeginLoop(Event* event) {
    camera.zoom(3);
    camera.calcView();
    camera.calcProj(800.0f/600.0f);
    createDefaultCube();
    load();
    event->print(module_name);
}

void EditSpace::onStartFrame(Event* event) {
    double time = glfwGetTime();
    float s = math::rads(75*time);
    camera.rotateInc(0.01f, 0.01f);
    camera.zoom(4+sin(s));
    camera.calcView();
}

void EditSpace::onDrawFrame(Event* event) {
    draw();
}

void EditSpace::onEndFrame(Event* event) {
    
}