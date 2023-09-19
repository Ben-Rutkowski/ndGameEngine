#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    // Events
    setEvent(Data::DEBUG, {&EditSpace::onDebug});
    setEvent(Data::BEGIN_LOOP, {&EditSpace::onBeginLoop});
    setEvent(Data::START_FRAME, {&EditSpace::onStartFrame});
    setEvent(Data::DRAW_FRAME, {&EditSpace::onDrawFrame});
    setEvent(Data::END_FRAME, {&EditSpace::onEndFrame});
    setEvent(Data::RESIZE_FRAME, {&EditSpace::onResizeFrame});
    setEvent(Data::RESIZE_WINDOW, {&EditSpace::onResizeWindow});
    setEvent(Data::SCROLL, {&EditSpace::onScroll});
    setEvent(Data::RIGHT_MOUSE_CLICK, {&EditSpace::onRightMouseClick});
    setEvent(Data::RIGHT_MOUSE_HOLD, {&EditSpace::onRightMouseHold});
    setEvent(Data::LEFT_MOUSE_CLICK, {&EditSpace::onLeftMouseClick});
    setEvent(Data::LEFT_MOUSE_HOLD, {&EditSpace::onLeftMouseHold});
    setEvent(Data::LEFT_MOUSE_RELEASE, {&EditSpace::onLeftMouseRelease});
    setEvent(Data::W_KEY, {&EditSpace::onWKey});
    setEvent(Data::S_KEY, {&EditSpace::onSKey});
}

void EditSpace::runEvent(Event* event) {
    // event_interface(event);
    runEventInLayer(event);
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

    // event_interface.manager->propogateEvent(module_name, Data::CLICK_DEBUG_TIMER);
    selectPointsBox();
    // event_interface.manager->propogateEvent(module_name, Data::DELTA_DEBUG_TIMER);
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
    event_interface.manager->propogateEvent(module_name, Data::CLICK_DEBUG_TIMER);
    meshes[0].debug();
    event_interface.manager->propogateEvent(module_name, Data::DELTA_DEBUG_TIMER);
}