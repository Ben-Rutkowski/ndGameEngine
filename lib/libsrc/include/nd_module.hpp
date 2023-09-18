#ifndef ND_MODULE_NEW_HPP
#define ND_MODULE_NEW_HPP

#include "event_manager.hpp"

struct DimensionCacheTEMPORARY {
    int fw, fh;
    int ww, wh;
    float fdelta;

    float FW() { return (float)fw; }
    float FH() { return (float)fh; }
    float WW() { return (float)ww; }
    float WH() { return (float)wh; }
};

class StateCacheTEMPORARY {
private:
    std::vector<bool> states;

public:
    StateCacheTEMPORARY(int n) { states.assign(n, false); }
    bool operator[](int i)      { return states[i]; }
    void set(int i, bool value) { states[i] = value; }
};

// ======== Module Reference ========
class ndModule {
protected:
    Module module_name;
    DimensionCacheTEMPORARY dcache;
    StateCacheTEMPORARY     scache;

public:
    ndModule(Module mod_in, int state_len)
        :module_name{ mod_in },
        scache(state_len) {}

public:
    virtual void setManagerPtr(EventManager* ptr) = 0;
    virtual bool requestBool(Request request) { return false; }
    virtual void runEvent(Event* event) = 0;
};

// ======== Types =========

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

// ======== Module Instance ========
template<typename MOD>
class ndModuleInstance : public ndModule {
protected:
    EventInterface<MOD> event_interface;

public:
    ndModuleInstance(Module mod_in, int state_len)
        :ndModule(mod_in, state_len) {}

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
        // else {
        //     std::cout << "Event Callback Not Set :: " << std::endl;
        //     event->print();
        // }
    }
};

#endif