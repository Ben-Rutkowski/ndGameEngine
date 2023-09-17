#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    event_interface.setCallback(Data::DEBUG,       PACK(EditSpace::onDebug));
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
    event_interface.setCallback(Data::W_KEY,              PACK(EditSpace::onWKey));
    event_interface.setCallback(Data::S_KEY,              PACK(EditSpace::onSKey));
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
    camera.rightClick(mouse_x, mouse_y);
}

void EditSpace::onRightMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    camera.rightDrag(mouse_x, mouse_y);
}

void EditSpace::onLeftMouseClick(Event* event) {
    scache.set(esLEFT_MOUSE, true);
    vec2 click = event->getVec2();

    select_box.grab(click);
    selectFacesClick(click);
}

void EditSpace::onLeftMouseHold(Event* event) {
    vec2 drag = event->getVec2();
    
    select_box.drag(drag);
}

void EditSpace::onLeftMouseRelease(Event* event) {
    scache.set(esLEFT_MOUSE, false);

    Event startTimer(module_name, Data::CLICK_DEBUG_TIMER);
    Event checkTimer(module_name, Data::DELTA_DEBUG_TIMER);
    event_interface.runEvent(&startTimer);
    selectPointsBox();
    event_interface.runEvent(&checkTimer);
}

void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.scroll(offset);
}

void EditSpace::onWKey(Event* event) {
    float T = 1.05f;

    mat4 trans = mat4::scale(T);

    std::vector<Id> points({4, 5, 6, 7});

    // meshes[0].transformPoints(points, trans);
    meshes[0].transformPoints(trans);
}

void EditSpace::onSKey(Event* event) {
    float T = 0.95f;

    mat4 trans = mat4::scale(T);

    std::vector<Id> points({4, 5, 6, 7});

    // meshes[0].transformPoints(points, trans);
    meshes[0].transformPoints(trans);
}

void EditSpace::onDebug(Event* event) {
    Event startTimer(module_name, Data::CLICK_DEBUG_TIMER);
    Event checkTimer(module_name, Data::DELTA_DEBUG_TIMER);
    event_interface.runEvent(&startTimer);
    meshes[0].debug();
    event_interface.runEvent(&checkTimer);
}