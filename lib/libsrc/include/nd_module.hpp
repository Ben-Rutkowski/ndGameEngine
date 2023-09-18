#ifndef ND_MODULE_NEW_HPP
#define ND_MODULE_NEW_HPP

#include "event_manager.hpp"

// ======== Module Reference ========
class ndModule {
protected:
    Module module_name;

public:
    ndModule(Module mod_in)
        :module_name{ mod_in } {}

public:
    virtual void setManagerPtr(EventManager* ptr) = 0;
    virtual bool requestBool(Request request) { return false; }
    virtual void runEvent(Event* event) = 0;
};

// ======== Caches =========

template<typename T>
struct onEventFunc {
    EventCallType<T> func;
    bool is_null = true;
};

template<typename T>
struct EventInterface {
typedef EnumMap<Data, onEventFunc<T>, EVENT_DATA_LEN> FunctionArray;

    FunctionArray on_event;
    EventManager* manager;
};

struct DimensionCache {
    int fw, fh;
    int ww, wh;
    float fdelta;

    float FW() { return (float)fw; }
    float FH() { return (float)fh; }
    float WW() { return (float)ww; }
    float WH() { return (float)wh; }
};

template<int N>
class StateCache {
private:
    std::array<bool,N> states;

public:
    StateCache()                { std::fill_n(states.begin(), N, false); }
    bool operator[](int i)      { return states[i]; }
    void set(int i, bool value) { states[i] = value; }
};

// ======== Module Instance ========
template<typename MOD, int N>
class ndModuleInstance : public ndModule {
protected:
    EventInterface<MOD> event_interface;
    DimensionCache      dcache;
    StateCache<N>       scache;

public:
    ndModuleInstance(Module mod_in)
        :ndModule(mod_in) {}

    void setManagerPtr(EventManager* ptr) {
        event_interface.manager = ptr;
    }

    void setEvent(Data data, EventCallType<MOD> func) {
        event_interface.on_event.addEntry(
            data, onEventFunc<MOD>{ func, false }
        );
    }

    void runEventInLayer(Event* event) {
        Data data = event->getData();
        onEventFunc<MOD> on_event = event_interface.on_event.get(data);
        if (!on_event.is_null) {
            on_event.func((MOD*)this, event);
        }
    }
};

#endif