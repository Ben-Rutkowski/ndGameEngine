/*

The window layer handles all of the window operations.
The window layer must attatch an event manager. All
GLFW intialization must be done before creating a window.

*/

#include "glad_glfw.hpp"
#include "event_manager.hpp"
#include "stopwatch.hpp"
#include "edit_space.hpp"

#ifndef WINDOW_HPP
#define WINDOW_HPP

// ================ KeyState ================
struct KeyState {
    bool right_mouse_hold;
    bool left_mouse_hold;
    KeyState()
        :right_mouse_hold{ false },
        left_mouse_hold{ false } {}
};

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
class ndWindow {
private: static Module module_name;
// --- Layers ---
private:
    EventInterface event_interface;
    Clock clock;
    EditSpace* edit_space;
    
// --- Attributes ---
private:
    int frame_width, frame_height;
    float frame_delta;
    GLFWwindow* glfw_window;
    KeyState hold_keys;

// --- Constructors and Initialization ---
public:
    ndWindow(int width, int height, const char* title);
    void setManagerPtr(EventManager* ptr);
    void linkEditSpace(EditSpace* edit_space_ptr);

// --- Gets and Sets ---
public:
    bool shouldClose();
    void setShouldClose(bool value);
    double getTime();

private:
    bool isKeyPress(int key);
    bool isMousePress(int mouse);

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
    void onDrawFrame(Event* event);
    void onEndFrame(Event* event);
    void onEscapeKey(Event* event);
    void onCloseApp(Event* event);
    void onResize(Event* event);

// --- GLFW CALLBACKS ---
private:
    static void framebufferResizeCallback(GLFWwindow*, int, int);
    static void scrollCallback(GLFWwindow*, double, double);

    static EventManager* getManager(GLFWwindow* window);
};

#endif