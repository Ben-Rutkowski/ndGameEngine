#ifndef MODULE_TEMPLATE_HPP
#define MODULE_TEMPLATE_HPP

#include "event_manager.hpp"

struct DimensionCacheOld {
    int fw, fh;
    int ww, wh;
    float fdelta;

    float FW() { return (float)fw; }
    float FH() { return (float)fh; }
    float WW() { return (float)ww; }
    float WH() { return (float)wh; }
};

class StateCacheOld {
private:
    std::vector<bool> states;

public:
    StateCacheOld(int n) { states.assign(n, false); }
    bool operator[](int i)      { return states[i]; }
    void set(int i, bool value) { states[i] = value; }
};

class ndModuleOld {
protected:
    Module         module_name;
    DimensionCacheOld dcache;
    StateCacheOld     scache;
    EventInterfaceOld event_interface;

public:
    ndModuleOld(Module mod_in, int state_len)
        :module_name{ mod_in },
        scache(state_len) {}

// --- Manager Init ---
public:
    virtual void setManagerPtr(EventManagerOld* ptr) = 0;

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