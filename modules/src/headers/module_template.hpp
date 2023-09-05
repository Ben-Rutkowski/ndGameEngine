#ifndef MODULE_TEMPLATE_HPP
#define MODULE_TEMPLATE_HPP

#include "event_manager.hpp"

struct StateCache {
    bool opperation1 = false;
    bool opperation2 = false;

    float frame_width, frame_height;
    float window_width, window_height;
    float frame_delta;
};

class ModuleTemplate {
protected:
    Module         module_name;
    StateCache     state_cache;
    EventInterface event_interface;

public:
    ModuleTemplate(Module mod_in)
        :module_name{ mod_in } {}

// --- Manager Init ---
public:
    virtual void setManagerPtr(EventManager* ptr) = 0;

protected:
    virtual void setCallbacks() = 0;

// --- Event Interface ---
public:
    virtual void runEvent(Event* event) = 0;
};

#endif