#ifndef MODULE_TEMPLATE_HPP
#define MODULE_TEMPLATE_HPP

#include "event_manager.hpp"

struct StateCache {
    bool opperation1 = false;
    bool opperation2 = false;

    int frame_width, frame_height;
    int window_width, window_height;
    float frame_delta;
};

class ndModule {
protected:
    Module         module_name;
    StateCache     state_cache;
    EventInterface event_interface;

public:
    ndModule(Module mod_in)
        :module_name{ mod_in } {}

// --- Manager Init ---
public:
    virtual void setManagerPtr(EventManager* ptr) = 0;

protected:
    virtual void setCallbacks() = 0;

// --- Requests ---
public:
    virtual bool requestBool(Request request) { return false; }

// --- Event Interface ---
public:
    virtual void runEvent(Event* event) = 0;
};

#endif