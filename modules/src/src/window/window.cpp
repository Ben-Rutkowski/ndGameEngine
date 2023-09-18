#include <iostream>
#include "window.hpp"

// === Constructors and Initialization ===
ndWindow::ndWindow(int width, int height, const char* title)
    :ndModuleOld(Module::WINDOW, wLEN),
    glfw_window{ nullptr } {

    dcache.fw = width;
    dcache.fh = height;

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

void ndWindow::setManagerPtr(EventManagerOld* ptr) {
    glfwSetWindowUserPointer(glfw_window, ptr);
    event_interface.linkManager(ptr);
}

void ndWindow::linkEditSpace(ndModuleOld* edit_space_ptr) {
    edit_space = edit_space_ptr;
    edit_space->setManagerPtr(event_interface.ptr());
}

// === Gets and Sets ===
bool ndWindow::requestBool(Request request) {
    if (request == Request::SHOULD_CLOSE) {
        return glfwWindowShouldClose(glfw_window);
    } else {
        return false;
    }
}

// === Private ===
bool ndWindow::isKeyPress(int key) {
    return glfwGetKey(glfw_window, key) == GLFW_PRESS;
}

vec2 ndWindow::mousePos() {
    double mouse_x, mouse_y;
    glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
    return vec2::screenToClip(mouse_x, mouse_y, dcache.WW(), dcache.WH());
}

wState ndWindow::mouseState(wState button, int glfw_button) {
    bool press = glfwGetMouseButton(glfw_window, glfw_button) == GLFW_PRESS;
    bool hold  = scache[button];

    if (press) {
        if (!hold) {
            scache.set(button, true);
            return wCLICK;
        } else {
            return wHOLD;
        }
    } else if (hold) {
        scache.set(button, false);
        return wRELEASE;
    } else {
        return w_null;
    }
}