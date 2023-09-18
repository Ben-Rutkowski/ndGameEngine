/*

The Application layer has the main loop. It also contains
the event callback function which propogates it to the lower
layers.

The application has the base for the event propogation funciton.
All other layers have a pointer to the application's event_manager
without knowing what the application layer is. The event manager 
calls the propogate event funciton which distributes it to the
other layers.

ATTRIBUTES
-- event_manager: The central event manager of the application.
-- window:        A pointer to the window layer.

-- static module_name: The name of the module.

CONSTRUCTORS AND INITIALIZATION
-- constructor(): Creates an app layer and sets the callback anchor
    functions for the event_manager.

-- attatchWindow: Sets a window pointer and gives the window the event_manager
    pointer.

RUNTIME
-- startApp: Starts the main loop for the application layer.

EVENTS:
-- propogateEvent: CALLBACK Takes an event pointer and distributes it
    throughout the stack.

-- pollEvents: Empties the event_manager queue by calling the event_manager
    method pollEvents.

*/

#include "nd_module_old.hpp"
#include "event_manager.hpp"

// #include "nd_module_new.hpp"
// #include "event_manager_new.hpp"

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

class ndApp
{
private:
    static Module module_name;

// --- Layers ---
private:
    EventManagerOld event_manager;
    ndModuleOld* window;

    // EventManager<ndApp> event_manager_new;
    // ndModule* test;

// --- Constructors and Initialization ---
public:
    ndApp();
    void linkWindow(ndModuleOld* window_ptr);

    // void linkTest(ndModule* test_ptr);

// --- Runtime ---
public:
    void startApp();
    // void debug();

// --- Events ---
public:
    void propogateEvent(Event* event);

private:
    void beginLoop();
    void startFrame();
    void drawFrame();
    void endFrame();
    void pollEvents();

public:
    static void propogateEventCallback(void* app_ptr, Event* event);
};

#endif