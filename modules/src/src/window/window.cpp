#include <iostream>
#include "window.hpp"

// === Constructors and Initialization ===
ndWindow::ndWindow(int width, int height, const char* title)
    :ndModuleInstance<ndWindow, wLEN>(Module::WINDOW),
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

void ndWindow::setManagerPtr(EventManager* ptr) {
    event_interface.manager = ptr;
    glfwSetWindowUserPointer(glfw_window, ptr);
}

void ndWindow::linkEditSpace(ndModule* edit_space_ptr) {
    edit_space = edit_space_ptr;
    edit_space->setManagerPtr(event_interface.manager);
}

// === Gets and Sets ===
bool ndWindow::requestBool(Request request) {
    bool out = false;
    if (request == Request::SHOULD_CLOSE) {
        out = glfwWindowShouldClose(glfw_window);
    } 
    return out;
}

float ndWindow::requestFloat(Request request) {
    float out = 0.0f;
    if (request == Request::FRAME_DELTA) {
        out = clock.delta(Watch::FRAME_DELTA);
        clock.click(Watch::FRAME_DELTA);
    }
    return out;
}

vec4 ndWindow::requestVec4(Request request) {
    vec4 out(0.0f);
    if (request == Request::MOUSE_POSITION) {
        vec2 mouse = mousePos();
        out = vec4({mouse[0], mouse[1], 0.0f, 0.0f});
    }
    return out;
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

wState ndWindow::keyState(wState key, int glfw_key) {
    bool press = isKeyPress(glfw_key);
    bool hold  = scache[key];

    if (press) {
        if (!hold) {
            scache.set(key, true);
            return wPRESS;
        } else {
            return wHOLD;
        }
    } else if (hold) {
        scache.set(key, false);
        return wRELEASE;
    } else {
        return w_null;
    }
}