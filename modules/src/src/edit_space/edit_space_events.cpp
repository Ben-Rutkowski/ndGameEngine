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

vec3 toProj(vec4 v, mat4 proj) {
    vec4 out = proj*v;
    float w = 1.0f/out[3];
    return vec3({
        out[0]*w, out[1]*w, out[2]*w
    });
}

vec3 toCoord(vec4 v, mat4 mat) {
    vec4 out = mat*v;
    float s = 1.0f/out[2];
    return vec3({
        out[0]*s, out[1]*s, s
    });
}

bool test(vec4 v, mat4 select) {
    vec4 p = select*v;
    float s = 1.0f/p[2];
    float x = p[0]*s;
    float y = p[1]*s;

    return s>0.0f && x>=0.0f && y>=0.0f && x<=1.0f && y<=1.0f;
}

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
    state_cache.frame_width  = event->getInt(0);
    state_cache.frame_height = event->getInt(1);

    float ratio = (float)state_cache.frame_width/(float)state_cache.frame_height;
    camera.calcProj(ratio);
}

void EditSpace::onResizeWindow(Event* event) {
    state_cache.window_width  = event->getInt(0);
    state_cache.window_height = event->getInt(1);
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
        (float)state_cache.window_width,
        (float)state_cache.window_height
    );

    state_cache.opperation1 = true;

}

void EditSpace::onLeftMouseHold(Event* event) {
    float mouse_x = event->getFloat(0);
    float mouse_y = event->getFloat(1);

    select_box.drag(
        vec2({mouse_x, mouse_y}),
        (float)state_cache.window_width,
        (float)state_cache.window_height
    );
}

void EditSpace::onLeftMouseRelease(Event* event) {
    state_cache.opperation1 = false;

    mat4 model = meshes[0].getModel();
    mat4 select_mat = camera.selectMatProj(model, select_box.getBR(), select_box.getTL());
    select_box.setSelectMatProj(select_mat);

    vec4 point;
    for (int i=0; i<8; i++) {
        point = meshes[0].getPoint(i);
        if (select_box.checkSelect(point)) { std::cout << i << " "; }
    }
    std::cout << std::endl;
}

void EditSpace::onScroll(Event* event) {
    float offset = event->getFloat(1);
    camera.zoom(offset);
    camera.calcView();
}