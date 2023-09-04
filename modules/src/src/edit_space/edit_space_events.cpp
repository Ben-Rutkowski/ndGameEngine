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

void EditSpace::runEventEditSpace(Event* event) {
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

    event->print(module_name);
}

void EditSpace::onStartFrame(Event* event) {
    
}

void EditSpace::onDrawFrame(Event* event) {
    draw();
}

void EditSpace::onEndFrame(Event* event) {
    
}

void EditSpace::onResizeFrame(Event* event) {
    int frame_width  = event->getInt(0);
    int frame_height = event->getInt(1);

    float ratio = (float)frame_width/(float)frame_height;
    camera.calcProj(ratio);
}

void EditSpace::onResizeWindow(Event* event) {
    int window_width_in  = event->getInt(0);
    int window_height_in = event->getInt(1);

    window_width  = (float)window_width_in;
    window_height = (float)window_height_in;
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

    select_box.grab(vec2({mouse_x, mouse_y}), window_width, window_height);
    draw_select = true;

    // camera.grab(mouse_x, mouse_y);
}

void EditSpace::onLeftMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);

    select_box.drag(vec2({mouse_x, mouse_y}), window_width, window_height);
    
    // select_box.draw(select_box_shader);
    // vec2 delta = camera.moveMouse(mouse_x, mouse_y);
    // camera.transOrigin(delta);
    // camera.calcView();
}

void EditSpace::onLeftMouseRelease(Event* event) {
    draw_select = false;
}

void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.zoom(offset);
    camera.calcView();
}