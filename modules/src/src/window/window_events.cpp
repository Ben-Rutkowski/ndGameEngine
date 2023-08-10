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

    // GLFW Callbacks
    glfwSetFramebufferSizeCallback(glfw_window, framebufferResizeCallback);
}

// ================ On Events ================

void ndWindow::onCollectMenuKeys(Event* event) {
    CODE(escape_key, Data::ESCAPE_KEY)

    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        event_interface.queueEvent(escape_key);
    }
}

void ndWindow::onStartFrame(Event* event) {
    CODE(collect_menu_keys, Data::COLLECT_MENU_KEYS)

    glfwSwapBuffers(glfw_window);

    Event menu_event(collect_menu_keys);
    event_interface.runEvent(&menu_event);
}

void ndWindow::onEscapeKey(Event* event) {
    CODE(close_app, Data::CLOSE_APP)
    
    event_interface.queueEvent(close_app);
}

void ndWindow::onCloseApp(Event* event) {
    // --- Actions ---
    setShouldClose(true);

    std::cout << "Close" << std::endl;
}

void ndWindow::onResize(Event* event) {
    frame_width  = event->getInt(0);
    frame_height = event->getInt(1);

    glViewport(0, 0, frame_width, frame_height);

    std::cout << "Resize: ";
    std::cout << frame_width << ", " << frame_height << std::endl;
}

// === GLFW CALLBACKS ===
void ndWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    CODE2I(framesize, Data::RESIZE)
    EventManager* event_manager = getManager(window);

    Event2i event(framesize, vec2i(width, height));
    event_manager->runEvent(&event);
}

EventManager* ndWindow::getManager(GLFWwindow* window) {
    void* void_ptr = glfwGetWindowUserPointer(window);
    return static_cast<EventManager*>(void_ptr);
}