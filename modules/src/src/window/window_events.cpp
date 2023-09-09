#include <iostream>
#include "window.hpp"

// === Event Interface ===

void ndWindow::runEvent(Event* event) {
    event_interface(event);
    edit_space->runEvent(event);
}

void ndWindow::setCallbacks() {
    // Event Callbacks
    event_interface.setCallback(Data::COLLECT_MENU_KEYS, PACK(ndWindow::onCollectMenuKeys));
    event_interface.setCallback(Data::START_FRAME, PACK(ndWindow::onStartFrame));
    event_interface.setCallback(Data::DRAW_FRAME,  PACK(ndWindow::onDrawFrame));
    event_interface.setCallback(Data::ESCAPE_KEY,  PACK(ndWindow::onEscapeKey));
    event_interface.setCallback(Data::CLOSE_APP,   PACK(ndWindow::onCloseApp));
    event_interface.setCallback(Data::RESIZE_FRAME, PACK(ndWindow::onResizeFrame));
    event_interface.setCallback(Data::RESIZE_WINDOW, PACK(ndWindow::onResizeWindow));
    event_interface.setCallback(Data::BEGIN_LOOP,  PACK(ndWindow::onBeginLoop));
    event_interface.setCallback(Data::END_FRAME,   PACK(ndWindow::onEndFrame));

    // GLFW Callbacks
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
    double mouse_x;
    double mouse_y;
    vec2 mouse;

    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        event_interface.queueEvent(module_name, Data::ESCAPE_KEY);
    }
    if (isMousePress(GLFW_MOUSE_BUTTON_RIGHT)) {
        glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
        if ( scache[wRIGHT_MOUSE] ) {
            event_interface.queueEvent2f(
                module_name, Data::RIGHT_MOUSE_HOLD, vec2({(float)mouse_x, (float)mouse_y})
            );
        } else {
            scache.set(wRIGHT_MOUSE, true);
            event_interface.queueEvent2f(
                module_name, Data::RIGHT_MOUSE_CLICK, vec2({(float)mouse_x, (float)mouse_y})
            );
        }
    } else {
        scache.set(wRIGHT_MOUSE, false);
    }
    if (isMousePress(GLFW_MOUSE_BUTTON_LEFT)) {
        mouse = mousePos();
        if ( scache[wLEFT_MOUSE] ) {
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_HOLD, mouse
            );
        } else {
            scache.set(wLEFT_MOUSE, true);
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_CLICK, mouse
            );
        }
    } 

    if (scache[wLEFT_MOUSE]) {
        if (!isMousePress(GLFW_MOUSE_BUTTON_LEFT)) {
            mouse = mousePos();
            scache.set(wLEFT_MOUSE, false);
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_RELEASE, vec2({(float)mouse_x, (float)mouse_y})
            );
        }
    }
}

void ndWindow::onStartFrame(Event* event) {
    // OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Clock
    dcache.fdelta = clock.delta(Watch::FRAME_DELTA);
    clock.click(Watch::FRAME_DELTA);

    Event menu_event(module_name, Data::COLLECT_MENU_KEYS);
    event_interface.runEvent(&menu_event);
}

void ndWindow::onDrawFrame(Event* event) {

}

void ndWindow::onEndFrame(Event* event) {
    // OpenGL
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();

    // Display Framerate
    if (clock.check(Watch::FRAMERATE, 3.0)) {
        std::cout << clock.rate(Watch::FRAMERATE) << std::endl;
        clock.click(Watch::FRAMERATE);
    }
}

void ndWindow::onEscapeKey(Event* event) {
    event_interface.queueEvent(module_name, Data::CLOSE_APP);
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

// === Private ===
void ndWindow::queueFrameResize() {
    int width, height;
    glfwGetFramebufferSize(glfw_window, &width, &height);
    event_interface.queueEvent2i(module_name, Data::RESIZE_FRAME, vec2i({width, height}));
}

void ndWindow::queueWindowResize() {
    int width, height;
    glfwGetWindowSize(glfw_window, &width, &height);
    event_interface.queueEvent2i(module_name, Data::RESIZE_WINDOW, vec2i({width, height}));
}

// === GLFW CALLBACKS ===
void ndWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    Event2i event(Module::WINDOW, Data::RESIZE_FRAME, width, height);
    getManager(window)->runEvent(&event);
}

void ndWindow::windowResizeCallback(GLFWwindow* window, int width, int height) {
    Event2i event(Module::WINDOW, Data::RESIZE_WINDOW, width, height);
    getManager(window)->runEvent(&event);
}

void ndWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Event2f event(Module::WINDOW, Data::SCROLL, vec2({(float)xoffset, (float)yoffset}));
    getManager(window)->runEvent(&event);
}

EventManager* ndWindow::getManager(GLFWwindow* window) {
    void* void_ptr = glfwGetWindowUserPointer(window);
    return static_cast<EventManager*>(void_ptr);
}