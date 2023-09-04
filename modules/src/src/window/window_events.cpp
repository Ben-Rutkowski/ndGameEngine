#include <iostream>
#include "window.hpp"

// === Event Interface ===

void ndWindow::runEventWindow(Event* event) {
    event_interface(event);
    edit_space->runEventEditSpace(event);
}

void ndWindow::setCallbacks() {
    // Event Callbacks
    event_interface.setCallback(Data::COLLECT_MENU_KEYS, PACK(ndWindow::onCollectMenuKeys));
    event_interface.setCallback(Data::START_FRAME, PACK(ndWindow::onStartFrame));
    event_interface.setCallback(Data::DRAW_FRAME,  PACK(ndWindow::onDrawFrame));
    event_interface.setCallback(Data::ESCAPE_KEY,  PACK(ndWindow::onEscapeKey));
    event_interface.setCallback(Data::CLOSE_APP,   PACK(ndWindow::onCloseApp));
    event_interface.setCallback(Data::RESIZE_FRAME, PACK(ndWindow::onResizeFrame));
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

    event->print(module_name);
}

void ndWindow::onCollectMenuKeys(Event* event) {
    double mouse_x;
    double mouse_y;

    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        event_interface.queueEvent(module_name, Data::ESCAPE_KEY);
    }
    if (isMousePress(GLFW_MOUSE_BUTTON_RIGHT)) {
        glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
        if ( hold_keys.right_mouse_hold ) {
            event_interface.queueEvent2f(
                module_name, Data::RIGHT_MOUSE_HOLD, vec2({(float)mouse_x, (float)mouse_y})
            );
        } else {
            hold_keys.right_mouse_hold = true;
            event_interface.queueEvent2f(
                module_name, Data::RIGHT_MOUSE_CLICK, vec2({(float)mouse_x, (float)mouse_y})
            );
        }
    } else { hold_keys.right_mouse_hold = false; }
    if (isMousePress(GLFW_MOUSE_BUTTON_LEFT)) {
        glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
        if ( hold_keys.left_mouse_hold ) {
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_HOLD, vec2({(float)mouse_x, (float)mouse_y})
            );
        } else {
            hold_keys.left_mouse_hold = true;
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_CLICK, vec2({(float)mouse_x, (float)mouse_y})
            );
        }
    } 

    if (hold_keys.left_mouse_hold) {
        if (!isMousePress(GLFW_MOUSE_BUTTON_LEFT)) {
            glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
            hold_keys.left_mouse_hold = false;
            event_interface.queueEvent2f(
                module_name, Data::LEFT_MOUSE_RELEASE, vec2({(float)mouse_x, (float)mouse_y})
            );
        }
    }
}

void ndWindow::onStartFrame(Event* event) {
    // OpenGL
    // glStencilMask(0xFF);
    // glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Clock
    frame_delta = clock.delta(Watch::FRAME_DELTA);
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

    event->print(module_name);
}

void ndWindow::onCloseApp(Event* event) {
    setShouldClose(true);

    event->print(module_name);
}

void ndWindow::onResizeFrame(Event* event) {
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

    Event2i event(module_name, Data::RESIZE_FRAME, width, height);
    event_manager->runEvent(&event);
}

void ndWindow::windowResizeCallback(GLFWwindow* window, int width, int height) {
    EventManager* event_manager = getManager(window);

    Event2i event(module_name, Data::RESIZE_WINDOW, width, height);
    event_manager->runEvent(&event);
}

void ndWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    EventManager* event_manager = getManager(window);

    Event2f event(module_name, Data::SCROLL, vec2({(float)xoffset, (float)yoffset}));
    event_manager->runEvent(&event);
}

EventManager* ndWindow::getManager(GLFWwindow* window) {
    void* void_ptr = glfwGetWindowUserPointer(window);
    return static_cast<EventManager*>(void_ptr);
}