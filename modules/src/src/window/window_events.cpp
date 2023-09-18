#include <iostream>
#include "window.hpp"

// === Event Interface ===

void ndWindow::runEvent(Event* event) {
    runEventInLayer(event);
    edit_space->runEvent(event);
    // event_interface(event);
}

void ndWindow::setCallbacks() {
    // Event Callbacks
    // event_interface.setCallback(Data::COLLECT_MENU_KEYS, PACK(ndWindow::onCollectMenuKeys));
    // event_interface.setCallback(Data::START_FRAME, PACK(ndWindow::onStartFrame));
    // event_interface.setCallback(Data::DRAW_FRAME,  PACK(ndWindow::onDrawFrame));
    // event_interface.setCallback(Data::ESCAPE_KEY,  PACK(ndWindow::onEscapeKey));
    // event_interface.setCallback(Data::CLOSE_APP,   PACK(ndWindow::onCloseApp));
    // event_interface.setCallback(Data::RESIZE_FRAME, PACK(ndWindow::onResizeFrame));
    // event_interface.setCallback(Data::RESIZE_WINDOW, PACK(ndWindow::onResizeWindow));
    // event_interface.setCallback(Data::BEGIN_LOOP,  PACK(ndWindow::onBeginLoop));
    // event_interface.setCallback(Data::END_FRAME,   PACK(ndWindow::onEndFrame));
    // event_interface.setCallback(Data::CLICK_DEBUG_TIMER,  PACK(ndWindow::onClickDebugTimer));
    // event_interface.setCallback(Data::DELTA_DEBUG_TIMER,   PACK(ndWindow::onDeltaDebugTimer));

    setEvent(Data::COLLECT_MENU_KEYS, {&ndWindow::onCollectMenuKeys});
    setEvent(Data::START_FRAME, {&ndWindow::onStartFrame});
    setEvent(Data::DRAW_FRAME, {&ndWindow::onDrawFrame});
    setEvent(Data::ESCAPE_KEY, {&ndWindow::onEscapeKey});
    setEvent(Data::CLOSE_APP, {&ndWindow::onCloseApp});
    setEvent(Data::RESIZE_FRAME, {&ndWindow::onResizeFrame});
    setEvent(Data::RESIZE_WINDOW, {&ndWindow::onResizeWindow});
    setEvent(Data::BEGIN_LOOP, {&ndWindow::onBeginLoop});
    setEvent(Data::END_FRAME, {&ndWindow::onEndFrame});
    setEvent(Data::CLICK_DEBUG_TIMER, {&ndWindow::onClickDebugTimer});
    setEvent(Data::DELTA_DEBUG_TIMER, {&ndWindow::onDeltaDebugTimer});

    // GLFW Callbacks
    // setUserPointerGLFW(event_interface.manager);
    glfwSetFramebufferSizeCallback(glfw_window, framebufferResizeCallback);
    glfwSetWindowSizeCallback(glfw_window, windowResizeCallback);
    glfwSetScrollCallback(glfw_window, scrollCallback);
}

// ================ On Events ================

void ndWindow::onBeginLoop(Event* event) {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glfwSwapInterval(1);

    queueFrameResize();
    queueWindowResize();
}

void ndWindow::onCollectMenuKeys(Event* event) {
    vec2 m = mousePos();
    vec4 mouse({m[0], m[1], 0.0f, 0.0f});
    wState mouse_state;

    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        // event_interface.queueEvent(module_name, Data::ESCAPE_KEY);
        event_interface.manager->queueEvent(module_name, Data::ESCAPE_KEY);
    }

    if (isKeyPress(GLFW_KEY_W)) {
        // event_interface.queueEvent(module_name, Data::W_KEY);
        event_interface.manager->queueEvent(module_name, Data::W_KEY);
    }

    if (isKeyPress(GLFW_KEY_S)) {
        // event_interface.queueEvent(module_name, Data::S_KEY);
        event_interface.manager->queueEvent(module_name, Data::S_KEY);
    }

    if (isKeyPress(GLFW_KEY_P) && !scache[wDEBUG]) {
        scache.set(wDEBUG, true);
        // event_interface.queueEvent(module_name, Data::DEBUG);
        event_interface.manager->queueEvent(module_name, Data::DEBUG);
    }

    mouse_state = mouseState(wRIGHT_MOUSE, GLFW_MOUSE_BUTTON_RIGHT);
    if (mouse_state == wCLICK) {
        // event_interface.queueEvent2f(module_name, Data::RIGHT_MOUSE_CLICK, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::RIGHT_MOUSE_CLICK, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::RIGHT_MOUSE_CLICK, mouse);
    } else if (mouse_state == wHOLD) {
        // event_interface.queueEvent2f(module_name, Data::RIGHT_MOUSE_HOLD, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::RIGHT_MOUSE_HOLD, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::RIGHT_MOUSE_HOLD, mouse);
    } else if (mouse_state == wRELEASE) {
        // event_interface.queueEvent2f(module_name, Data::RIGHT_MOUSE_RELEASE, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::RIGHT_MOUSE_RELEASE, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::RIGHT_MOUSE_RELEASE, mouse);
    }

    mouse_state = mouseState(wLEFT_MOUSE, GLFW_MOUSE_BUTTON_LEFT);
    if (mouse_state == wCLICK) {
        // event_interface.queueEvent2f(module_name, Data::LEFT_MOUSE_CLICK, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::LEFT_MOUSE_CLICK, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::LEFT_MOUSE_CLICK, mouse);
    } else if (mouse_state == wHOLD) {
        // event_interface.queueEvent2f(module_name, Data::LEFT_MOUSE_HOLD, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::LEFT_MOUSE_HOLD, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::LEFT_MOUSE_HOLD, mouse);
    } else if (mouse_state == wRELEASE) {
        // event_interface.queueEvent2f(module_name, Data::LEFT_MOUSE_RELEASE, mouse);
        // event_interface.manager->queueEvent2f(module_name, Data::LEFT_MOUSE_RELEASE, mouse);
        event_interface.manager->queueEvent4f(module_name, Data::LEFT_MOUSE_RELEASE, mouse);
    }
}

void ndWindow::onStartFrame(Event* event) {
    // OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Clock
    dcache.fdelta = clock.delta(Watch::FRAME_DELTA);
    clock.click(Watch::FRAME_DELTA);

    // Event menu_event(module_name, Data::COLLECT_MENU_KEYS);
    // event_interface.runEvent(&menu_event);
    // event_interface.manager->propogateEvent(&menu_event);
    event_interface.manager->propogateEvent(module_name, Data::COLLECT_MENU_KEYS);
}

void ndWindow::onDrawFrame(Event* event) {

}

void ndWindow::onEndFrame(Event* event) {
    // OpenGL
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();

    // Display Framerate
    if (clock.check(Watch::FRAMERATE, 3.0)) {
        // std::cout << clock.rate(Watch::FRAMERATE) << std::endl;
        clock.click(Watch::FRAMERATE);
    }
}

void ndWindow::onEscapeKey(Event* event) {
    // event_interface.queueEvent(module_name, Data::CLOSE_APP);
    event_interface.manager->queueEvent(module_name, Data::CLOSE_APP);
}

void ndWindow::onCloseApp(Event* event) {
    glfwSetWindowShouldClose(glfw_window, true);
}

void ndWindow::onResizeFrame(Event* event) {
    dcache.fw = event->getInt(0);
    dcache.fh = event->getInt(1);
    
    glViewport(0, 0, dcache.fw, dcache.fh);
}

void ndWindow::onResizeWindow(Event* event) {
    dcache.ww = event->getInt(0);
    dcache.wh = event->getInt(1);
}

void ndWindow::onClickDebugTimer(Event* event) {
    clock.click(Watch::DEBUG);
}

void ndWindow::onDeltaDebugTimer(Event* event) {
    float time = clock.delta(Watch::DEBUG);
    std::cout << "Delta Time: " << time*1000.0f << " ms" << std::endl;
}

// === Private ===
void ndWindow::queueFrameResize() {
    int width, height;
    glfwGetFramebufferSize(glfw_window, &width, &height);
    // event_interface.queueEvent2i(module_name, Data::RESIZE_FRAME, vec2i({width, height}));
    // event_interface.manager->queueEvent2i(module_name, Data::RESIZE_FRAME, vec2i({width, height}));
    event_interface.manager->queueEvent4i(module_name, Data::RESIZE_FRAME, vec4i({width, height, 0, 0}));
}

void ndWindow::queueWindowResize() {
    int width, height;
    glfwGetWindowSize(glfw_window, &width, &height);
    // event_interface.queueEvent2i(module_name, Data::RESIZE_WINDOW, vec2i({width, height}));
    // event_interface.manager->queueEvent2i(module_name, Data::RESIZE_WINDOW, vec2i({width, height}));
    event_interface.manager->queueEvent4i(module_name, Data::RESIZE_WINDOW, vec4i({width, height, 0, 0}));
}

// === GLFW CALLBACKS ===
void ndWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    // Event2i event(Module::WINDOW, Data::RESIZE_FRAME, width, height);
    // getManager(window)->propogateEvent(&event);

    getManager(window)->propogateEvent4i(
        Module::WINDOW, Data::RESIZE_FRAME, vec4i({width, height, 0, 0})
    );
}

void ndWindow::windowResizeCallback(GLFWwindow* window, int width, int height) {
    // Event2i event(Module::WINDOW, Data::RESIZE_WINDOW, width, height);
    // getManager(window)->propogateEvent(&event);

    getManager(window)->propogateEvent4i(
        Module::WINDOW, Data::RESIZE_WINDOW, vec4i({width, height, 0, 0})
    );
}

void ndWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Event2f event(Module::WINDOW, Data::SCROLL, vec2({(float)xoffset, (float)yoffset}));
    // getManager(window)->propogateEvent(&event);
    getManager(window)->propogateEvent4f(
        Module::WINDOW, Data::SCROLL, vec4({(float)xoffset, (float)yoffset, 0.0f, 0.0f})
    );
}

// EventManagerOld* ndWindow::getManager(GLFWwindow* window) {
//     void* void_ptr = glfwGetWindowUserPointer(window);
//     return static_cast<EventManagerOld*>(void_ptr);
// }

EventManager* ndWindow::getManager(GLFWwindow* window) {
    void* void_ptr = glfwGetWindowUserPointer(window);
    return static_cast<EventManager*>(void_ptr);
}