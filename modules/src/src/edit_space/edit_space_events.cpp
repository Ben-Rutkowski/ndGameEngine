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

    // debug_box.setVerts(
    //     vec4({-0.25f, -0.25f, 0.0f, 1.0f}),
    //     vec4({ 0.25f, -0.25f, 0.0f, 1.0f}),
    //     vec4({ 0.25f,  0.25f, 0.0f, 1.0f}),
    //     vec4({-0.25f,  0.25f, 0.0f, 1.0f})
    // );
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
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    vec2 click = event->getVec2();
    // === Vertex Select Box ===
    select_box.grab(vec2({mouse_x, mouse_y}));
    scache.set(esLEFT_MOUSE, true);


    mat4 model = meshes[0].getModel();
    mat4 view  = camera.getView();
    mat4 proj  = camera.getProj();
    mat4 to_clip = proj*view*model;

    meshes[0].selectFaces(click, to_clip);
}

void EditSpace::onLeftMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    // === Vertex Select Box ===
    select_box.drag(vec2({mouse_x, mouse_y}));
}

void EditSpace::onLeftMouseRelease(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);
    // === Vertex Select Box ===
    float RADIUS = 0.05f;
    scache.set(esLEFT_MOUSE, false);

    selectPointsBox();

    // mat4 model = meshes[0].getModel();
    // mat4 select_mat = camera.selectMatProj(model, select_box.getRoot(), select_box.getEnd());
    // if (!isfinite(select_mat.get(0,0))) {
    //     select_mat = camera.selectMatProj(
    //         model, vec2({mouse_x - RADIUS, mouse_y - RADIUS}),
    //         vec2({mouse_x + RADIUS, mouse_y + RADIUS}));
    // }
    // meshes[0].setSelectedPoints(select_mat);

    
}

void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.scroll(offset);
}

void EditSpace::onWKey(Event* event) {
    vec4 trans({
        0.0f, 0.01f, 0.0f, 1.0f
    });
    meshes[0].translateSelectPoints(trans);
}

void EditSpace::onSKey(Event* event) {
    vec4 trans({
        0.0f, -0.01f, 0.0f, 1.0f
    });
    meshes[0].translateSelectPoints(trans);
}