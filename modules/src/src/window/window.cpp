#include <iostream>
#include "window.hpp"

// === Constructors and Initialization ===
Module ndWindow::module_name = Module::WINDOW;

ndWindow::ndWindow(int width, int height, const char* title)
    :frame_width{ width },
    frame_height{ height },
    glfw_window{ nullptr } {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glfw_window) {
        std::cout << "FAILED TO CREATE WINDOW" << std::endl;
    }
    glfwMakeContextCurrent(glfw_window);

    gladLoadGL(glfwGetProcAddress);

    setCallbacks();
}

void ndWindow::setManagerPtr(EventManager* ptr) {
    glfwSetWindowUserPointer(glfw_window, ptr);
    event_interface.linkManager(ptr);
}

// === Gets and Sets ===
bool ndWindow::shouldClose() {
    return glfwWindowShouldClose(glfw_window);
}

void ndWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(glfw_window, value);
}

bool ndWindow::isKeyPress(int key) {
    return glfwGetKey(glfw_window, key) == GLFW_PRESS;
}

double ndWindow::getTime() {
    return glfwGetTime();
}