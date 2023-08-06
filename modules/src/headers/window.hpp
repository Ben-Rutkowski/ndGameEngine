/*

The window layer handles all of the window operations.
The window layer must attatch an event manager. All
GLFW intialization must be done before creating a window.

*/

#include "glad_glfw.hpp"
#include "event_manager.hpp"

#ifndef WINDOW_HPP
#define WINDOW_HPP

class ndWindow {
private:
    static Module mod;

// --- Events ---
private:
    EventInterface event_interface;
    
// --- Attributes ---
public:
    GLFWwindow* glfw_window;

// --- Constructors and Initialization ---
public:
    ndWindow(int width, int height, const char* title);
    void setManagerPtr(EventManager* ptr);

// --- Gets and Sets ---
public:
    bool shouldClose();
    void setShouldClose(bool value);

private:
    bool isKeyPress(int key);

// --- Interface ---
public:
    void swapBuffers();

// --- Events ---
public:
    void collectKeys();
    void runEventWindow(Event* event);

private:
    void setEventCalls();
    void queueEvent(EventType type, Data data);

    void onEscapeKey(Event* event);
    void onCloseApp(Event* event);
};

#endif