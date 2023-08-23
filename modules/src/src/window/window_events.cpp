#include <iostream>
#include "window.hpp"

// === Event Interface ===

void ndWindow::runEventWindow(Event* event) {
    event_interface(event);
}

void ndWindow::setCallbacks() {
    // Event Callbacks
    event_interface.setCallback(Data::COLLECT_MENU_KEYS, PACK(ndWindow::onCollectMenuKeys));
    event_interface.setCallback(Data::START_FRAME, PACK(ndWindow::onStartFrame));
    event_interface.setCallback(Data::ESCAPE_KEY,  PACK(ndWindow::onEscapeKey));
    event_interface.setCallback(Data::CLOSE_APP,   PACK(ndWindow::onCloseApp));
    event_interface.setCallback(Data::RESIZE,      PACK(ndWindow::onResize));
    event_interface.setCallback(Data::BEGIN_LOOP,  PACK(ndWindow::onBeginLoop));
    event_interface.setCallback(Data::END_FRAME,   PACK(ndWindow::onEndFrame));

    // GLFW Callbacks
    glfwSetFramebufferSizeCallback(glfw_window, framebufferResizeCallback);
}

// ================ On Events ================

void ndWindow::onBeginLoop(Event* event) {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glfwSwapInterval(1);

    event->print(module_name);
}

void ndWindow::onCollectMenuKeys(Event* event) {
    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        event_interface.queueEvent(module_name, Data::ESCAPE_KEY);
    }
}

void ndWindow::onStartFrame(Event* event) {
    // OpenGL
    glClear(GL_COLOR_BUFFER_BIT);

    // Clock
    frame_delta = clock.delta(Watch::FRAME_DELTA);
    clock.click(Watch::FRAME_DELTA);

    Event menu_event(module_name, Data::COLLECT_MENU_KEYS);
    event_interface.runEvent(&menu_event);
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

    event->print(module_name);
}

void ndWindow::onCloseApp(Event* event) {
    setShouldClose(true);

    event->print(module_name);
}

void ndWindow::onResize(Event* event) {
    frame_width  = event->getInt(0);
    frame_height = event->getInt(1);

    glViewport(0, 0, frame_width, frame_height);

    event->print(module_name);
    std::cout << "Framebuffer size: ";
    std::cout << frame_width << ", " << frame_height << std::endl;
}

// === GLFW CALLBACKS ===
void ndWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    EventManager* event_manager = getManager(window);

    Event2i event(module_name, Data::RESIZE, width, height);
    event_manager->runEvent(&event);
}

EventManager* ndWindow::getManager(GLFWwindow* window) {
    void* void_ptr = glfwGetWindowUserPointer(window);
    return static_cast<EventManager*>(void_ptr);
}