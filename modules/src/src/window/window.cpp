#include <iostream>
#include "window.hpp"

// === Constructors and Initialization ===
Module ndWindow::mod = Module::WINDOW;

ndWindow::ndWindow(int width, int height, const char* title):
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

    setEventCalls();
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

// === Interface ===
void ndWindow::swapBuffers() {
    glfwSwapBuffers(glfw_window);
}