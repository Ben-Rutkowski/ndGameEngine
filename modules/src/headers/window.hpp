#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "cocoa_interface.hpp"
#include "nd_module.hpp"

enum ndWindowState { ndWindowStateNull = 0,
    ndWindowStateLen
};

class ndWindowModule : public ndModule<ndWindowStateLen> {
// --- Attributes ---
private:
    ndWindow nd_window;

// --- Layers ---
private:
    ndModule* edit_space;

// --- Initialization ---
public:
    ndWindowModule(int width, int height, const char* title);
    void linkEditSpace(ndModule* edit_space_ptr);

    bool  requestBool(Request request);
    float requestFloat(Request request);

// --- Events ---
public:
    void runEvent(ndEvent* event);

private:
    void onBeginStartUp(ndEvent* event);
    void onEndStartUp(ndEvent* event);
    void onStartFrame(ndEvent* event);
    void onDrawWindowFrame(ndEvent* event);
    void onDraw(ndEvent* evnet);
    void onEndFrame(ndEvent* event);
    void onCloseApp(ndEvent* event);
    // void onCollectMenuKeys(ndEvent* event);
    // void onEscapeKey(ndEvent* event);
    // void onResizeFrame(ndEvent* event);
    // void onResizeWindow(ndEvent* event);
    // void onClickDebugTimer(ndEvent* event);
    // void onDeltaDebugTimer(ndEvent* event);

// --- Cocoa Callbacks ---
// private:
    // static void framebufferResizeCallback(ndWindow, int, int);
    // static void windowResizeCallback(ndWindow*, int, int);
    // static void scrollCallback(ndWindow*, double, double);
};

#endif