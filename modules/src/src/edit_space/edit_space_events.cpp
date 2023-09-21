#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    // Events
    setEvent(Data::DEBUG, {&EditSpace::onDebug});
    setEvent(Data::BEGIN_LOOP, {&EditSpace::onBeginLoop});
    setEvent(Data::START_FRAME, {&EditSpace::onStartFrame});
    setEvent(Data::DRAW_WINDOW_FRAME, {&EditSpace::onDrawWindowFrame});
    setEvent(Data::DRAW, {&EditSpace::onDraw});
    setEvent(Data::END_FRAME, {&EditSpace::onEndFrame});
    setEvent(Data::RESIZE_FRAME, {&EditSpace::onResizeFrame});
    setEvent(Data::RESIZE_WINDOW, {&EditSpace::onResizeWindow});
    setEvent(Data::SCROLL, {&EditSpace::onScroll});
    setEvent(Data::SHIFT_PRESS, {&EditSpace::onShiftPress}),
    setEvent(Data::SHIFT_RELEASE, {&EditSpace::onShiftRelease}),
    setEvent(Data::RIGHT_MOUSE_CLICK, {&EditSpace::onRightMouseClick});
    setEvent(Data::RIGHT_MOUSE_HOLD, {&EditSpace::onRightMouseHold});
    setEvent(Data::LEFT_MOUSE_CLICK, {&EditSpace::onLeftMouseClick});
    setEvent(Data::LEFT_MOUSE_HOLD, {&EditSpace::onLeftMouseHold});
    setEvent(Data::LEFT_MOUSE_RELEASE, {&EditSpace::onLeftMouseRelease});
    setEvent(Data::W_KEY, {&EditSpace::onWKey});
    setEvent(Data::S_KEY, {&EditSpace::onSKey});
    setEvent(Data::G_KEY_PRESS, {&EditSpace::onGKeyPress});
    setEvent(Data::G_KEY_RELEASE, {&EditSpace::onGKeyRelease});
}

void EditSpace::runEvent(Event* event) {
    runEventInLayer(event);
}

// === On Events ===
// === Loop Events ===
void EditSpace::onBeginLoop(Event* event) {
    // dcache.mousedx = 0.0f;
    // dcache.mousedy = 0.0f;
    // dcache.mousex  = 0.0f;
    // dcache.mousey  = 0.0f;

    selected_mesh = 0;

    camera.setDistance(6);
    camera.setRotate(math::rads(30.0f), math::rads(-20.0f));
    camera.calcView();
    camera.calcProj(800.0f/600.0f);
    createDefaultCube();
    load();
}

void EditSpace::onStartFrame(Event* event) {
    vec4 v = event->getVec4();
    // dcache.mousedx = v[0] - dcache.mousex;
    // dcache.mousedy = v[1] - dcache.mousey;
    // dcache.mousex = v[0];
    // dcache.mousey = v[1];
    dcache.setMouse(v[0], v[1]);
    dcache.fdelta = v[2];

    if (scache[esCARRY]) {
        translateSelectedPoints();
        // translateSelectedPoints(vec4::basis(1));
    }
}
void EditSpace::onDrawWindowFrame(Event* event) {
    window_frame.startDraw();
    // draw();
    drawFaceMode();
    window_frame.endDraw();
}
void EditSpace::onDraw(Event* event) {
    window_frame.draw(test_window_frame_shader);
}
void EditSpace::onEndFrame(Event* event) {}

// === Resize Events ===
void EditSpace::onResizeFrame(Event* event) {
    dcache.fw = event->getInt(0);
    dcache.fh = event->getInt(1);

    window_frame.resizeRelative(dcache.fw, dcache.fh);

    float ratio = dcache.FW()/dcache.FH();
    camera.calcProj(ratio);
}
void EditSpace::onResizeWindow(Event* event) {
    dcache.ww = event->getInt(0);
    dcache.wh = event->getInt(1);
}

// === Mouse Events ===
void EditSpace::onRightMouseClick(Event* event) {
    // float mouse_x = event->getFloat(0);
    // float mouse_y = event->getFloat(1);
    // camera.rightClick(mouse_x, mouse_y);
    camera.rightClick(dcache.mousex, dcache.mousey);
}
void EditSpace::onRightMouseHold(Event* event) {
    // float mouse_x = event->getFloat(0);
    // float mouse_y = event->getFloat(1);
    // camera.rightDrag(mouse_x, mouse_y);
    camera.rightDrag(dcache.mousex, dcache.mousey);
}

void EditSpace::onLeftMouseClick(Event* event) {
    scache.set(esLEFT_MOUSE, true);
    vec2 click = event->getVec2();
    select_box.grab(click);

    // if (!scache[esCARRY]) {
    //     if (!scache[esSHIFT]) {
    //         selectFacesClick(click);
    //     } else {
    //         selectFacesClickAdd(click);
    //     }
    // }
}
void EditSpace::onLeftMouseHold(Event* event) {
    vec2 drag = event->getVec2();
    
    select_box.drag(drag);
}
void EditSpace::onLeftMouseRelease(Event* event) {
    scache.set(esLEFT_MOUSE, false);

    if (scache[esCARRY]) {
        scache.set(esCARRY, false);
        meshes[0].clearSelectedPoints();
    } else if (!scache[esSHIFT]) {
        selectPointsBox();
    } else {
        selectPointsBoxAdd();
    } 
}

void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.scroll(offset);
}

// === Button Events ===
void EditSpace::onShiftPress(Event* event)
    { scache.set(esSHIFT, true); }
void EditSpace::onShiftRelease(Event* event)
    { scache.set(esSHIFT, false); }

void EditSpace::onWKey(Event* event) {
    float T = 1.05f;
    mat4 trans = mat4::scale(T);
    meshes[0].transformPoints(trans);
}
void EditSpace::onSKey(Event* event) {
    float T = 0.95f;
    mat4 trans = mat4::scale(T);
    meshes[0].transformPoints(trans);
}
void EditSpace::onGKeyPress(Event* event) {
    scache.set(esCARRY, true);
}
void EditSpace::onGKeyRelease(Event* event) {
    
}

// === Debug ===
void EditSpace::onDebug(Event* event) {
    event_interface.manager->propogateEvent(module_name, Data::CLICK_DEBUG_TIMER);
    meshes[0].debug();
    event_interface.manager->propogateEvent(module_name, Data::DELTA_DEBUG_TIMER);
}