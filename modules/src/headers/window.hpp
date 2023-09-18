/*

The window layer handles all of the window operations.
The window layer must attatch an event manager. All
GLFW intialization must be done before creating a window.

*/

#include "glad_glfw.hpp"
// #include "nd_module_old.hpp"
#include "nd_module.hpp"
#include "stopwatch.hpp"

#ifndef WINDOW_HPP
#define WINDOW_HPP

// ================ Clock ================
class Clock {
private:
    std::array<StopWatch,WATCH_LEN> watches;

public:
    Clock() {}

// --- Time ---
    double delta(Watch type);
    bool   check(Watch type, double min_interval);
    void   click(Watch type);

// --- Rate ---
    double rate(Watch type);

// --- Private ---
private:
    double     getTime();
    StopWatch& getWatch(Watch type);
};

// ================ ndWindow ================
enum wState { w_null = 0, wDEBUG,
    /* KEYS */  wLEFT_MOUSE, wRIGHT_MOUSE,
    /* STATE */ wCLICK, wHOLD, wRELEASE,
    wLEN
};

class ndWindow : public ndModuleInstance<ndWindow> {
// --- Layers ---
private:
    // ndModuleOld* edit_space;
    ndModule* edit_space;
    Clock     clock;
    
// --- Attributes ---
private:
    GLFWwindow* glfw_window;

// --- Constructors and Initialization ---
public:
    ndWindow(int width, int height, const char* title);
    void setUserPointerGLFW(EventManager* ptr);
    void linkEditSpace(ndModule* edit_space_ptr);

    // void setManagerPtr(EventManagerOld* ptr);
    // void linkEditSpace(ndModuleOld* edit_space_ptr);

// --- Gets and Sets ---
public:
    bool requestBool(Request request);

private:
    bool isKeyPress(int key);
    vec2 mousePos();
    wState mouseState(wState button, int glfw_button);

// --- Event Interface ---
public:
    void runEvent(Event* event);

private:
    void setCallbacks();

// --- On Events ---
public:
    void onBeginLoop(Event* event);
    void onCollectMenuKeys(Event* event);
    void onStartFrame(Event* event);
    void onDrawFrame(Event* event);
    void onEndFrame(Event* event);
    void onEscapeKey(Event* event);
    void onCloseApp(Event* event);
    void onResizeFrame(Event* event);
    void onResizeWindow(Event* event);
    void onClickDebugTimer(Event* event);
    void onDeltaDebugTimer(Event* event);

private:
    void queueFrameResize();
    void queueWindowResize();

// --- GLFW CALLBACKS ---
private:
    static void framebufferResizeCallback(GLFWwindow*, int, int);
    static void windowResizeCallback(GLFWwindow*, int, int);
    static void scrollCallback(GLFWwindow*, double, double);

    // static EventManagerOld* getManager(GLFWwindow* window);
    static EventManager* getManager(GLFWwindow* window);
};

#endif