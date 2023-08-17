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
    static Module module_name;

    int frame_width, frame_height;

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

// --- Event Interface ---
public:
    void runEventWindow(Event* event);

private:
    void setCallbacks();

// --- On Events ---
public:
    void onBeginLoop(Event* event);
    void onCollectMenuKeys(Event* event);
    void onStartFrame(Event* event);
    void onEndFrame(Event* event);
    void onEscapeKey(Event* event);
    void onCloseApp(Event* event);
    void onResize(Event* event);

// --- GLFW CALLBACKS ---
private:
    static void framebufferResizeCallback(GLFWwindow*, int, int);

    static EventManager* getManager(GLFWwindow* window);
};

#endif