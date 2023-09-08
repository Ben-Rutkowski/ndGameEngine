#ifndef MODULE_TEMPLATE_HPP
#define MODULE_TEMPLATE_HPP

#include "event_manager.hpp"

struct StateCache {
    bool opperation1 = false;
    bool opperation2 = false;

    int fw, fh;
    int ww, wh;
    float fdelta;

    float FW() { return (float)fw; }
    float FH() { return (float)fh; }
    float WW() { return (float)ww; }
    float WH() { return (float)wh; }
};

class ndModule {
protected:
    Module         module_name;
    StateCache     scache;
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