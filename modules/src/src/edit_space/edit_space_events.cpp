#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    event_interface.setCallback(Data::BEGIN_LOOP,  PACK(EditSpace::onBeginLoop));
    event_interface.setCallback(Data::START_FRAME, PACK(EditSpace::onStartFrame));
    event_interface.setCallback(Data::DRAW_FRAME,  PACK(EditSpace::onDrawFrame));
    event_interface.setCallback(Data::END_FRAME,   PACK(EditSpace::onEndFrame));
    event_interface.setCallback(Data::RESIZE_FRAME,      PACK(EditSpace::onResizeFrame));
    event_interface.setCallback(Data::RESIZE_WINDOW,     PACK(EditSpace::onResizeWindow));
    event_interface.setCallback(Data::SCROLL,            PACK(EditSpace::onScroll));
    event_interface.setCallback(Data::RIGHT_MOUSE_CLICK, PACK(EditSpace::onRightMouseClick));
    event_interface.setCallback(Data::RIGHT_MOUSE_HOLD,  PACK(EditSpace::onRightMouseHold));
    event_interface.setCallback(Data::LEFT_MOUSE_CLICK,  PACK(EditSpace::onLeftMouseClick));
    event_interface.setCallback(Data::LEFT_MOUSE_HOLD,   PACK(EditSpace::onLeftMouseHold));
    event_interface.setCallback(Data::LEFT_MOUSE_RELEASE, PACK(EditSpace::onLeftMouseRelease));
}

void EditSpace::runEvent(Event* event) {
    event_interface(event);
}

// === On Events ===
void EditSpace::onBeginLoop(Event* event) {
    camera.setDistance(6);
    camera.setRotate(math::rads(30.0f), math::rads(-20.0f));
    camera.calcView();
    camera.calcProj(800.0f/600.0f);
    createDefaultCube();
    load();
}

void EditSpace::onStartFrame(Event* event) {
    
}

void EditSpace::onDrawFrame(Event* event) {
    draw();
}

void EditSpace::onEndFrame(Event* event) {
    
}

void EditSpace::onResizeFrame(Event* event) {
    dcache.fw = event->getInt(0);
    dcache.fh = event->getInt(1);

    float ratio = dcache.FW()/dcache.FH();
    camera.calcProj(ratio);
}

void EditSpace::onResizeWindow(Event* event) {
    dcache.ww = event->getInt(0);
    dcache.wh = event->getInt(1);
}

void EditSpace::onRightMouseClick(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    camera.grab(mouse_x, mouse_y);
}

void EditSpace::onRightMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    vec2 delta = camera.moveMouse(mouse_x, mouse_y);
    float yaw_delta = asin(delta[0]/MOUSE_DISTANCE_FACTOR);
    float pitch_delta = asin(delta[1]/MOUSE_DISTANCE_FACTOR);

    if (abs(pitch_delta) >= math::rads(45.0f)) { pitch_delta = 0.0f; }
    if (abs(yaw_delta) >= math::rads(45.0f))   { yaw_delta = 0.0f; }

    camera.rotateInc(pitch_delta, yaw_delta);
    camera.calcView();
}

void EditSpace::onLeftMouseClick(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);

    select_box.grab(
        vec2({mouse_x, mouse_y}),
        dcache.WW(),
        dcache.WH()
    );

    scache.set(esLEFT_MOUSE, true);
}

void EditSpace::onLeftMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);

    select_box.drag(
        vec2({mouse_x, mouse_y}),
        dcache.WW(),
        dcache.WH()
    );
}

void EditSpace::onLeftMouseRelease(Event* event) {
    scache.set(esLEFT_MOUSE, false);

    mat4 model = meshes[0].getModel();
    mat4 select_mat = camera.selectMatProj(model, select_box.getRoot(), select_box.getEnd());

    meshes[0].setSelectedPoints(select_mat);
}


void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.zoom(offset);
    camera.calcView();
}