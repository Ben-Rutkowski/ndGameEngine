#ifndef ND_MODULE_HPP
#define ND_MODULE_HPP

#include "event_manager.hpp"
#include "cocoa_interface.hpp"

// ======== State Cache ========
template<int N>
class StateCache {
private:
    std::array<bool,N> states;

public:
    StateCache()                { std::fill_n(states.begin(), N, false); }
    bool operator[](int i)      { return states[i]; }
    void set(int i, bool value) { states[i] = value; }
};

// ======== Module ========
class ndModule {
protected:
    Module module_name;
    ndRoutine draw_routine;
    EventManager* event_manager;

public:
    ndModule(Module mod_in) : module_name{ mod_in } {}
    void setManagerPtr(EventManager* ptr) { event_manager = ptr; }
    void setDrawRoutine(ndRoutine routine) { draw_routine = routine; }

// --- Virtual Functions ---
public:
    virtual void  runEvent(ndEvent* event) = 0;
    virtual bool  requestBool(Request request)  { return false; }
    virtual float requestFloat(Request request) { return 0.0f; }
    // virtual vec4  requestVec4(Request requset)   { return vec4(0); }
};


// ======== Module Implement ========
template<int N>
class ndModuleImplement : public ndModule {
protected:
    StateCache<N> state_cache;

public:
    ndModuleImplement<N>(Module mod_in) : ndModule( mod_in ) {}
};

#endif
