#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "nd_module.hpp"

enum ndAppState { ndAppStateNull = 0,
    ndAppStateLen
};

class ndAppModule : public ndModule<ndAppStateLen> {
// --- Attributes ---
private:
    EventManager event_manager;
    ndModule*    window;

// --- Initialization ---
public: 
    ndAppModule();
    void linkWindow(ndModule* window_ptr);

// --- Runtime ---
public:
    void startApp();

// --- Events ---
public:
    void propogateEvent(ndEvent* event);
    void runEvent(ndEvent* event) {}

private:
    void beginLoop();
    void startFrame();
    void drawWindowFrame();
    void draw();
    void endFrame();
    void pollEvents();

public:
    static void propogateEventCallback(void* app_ptr, ndEvent* event);
};

#endif