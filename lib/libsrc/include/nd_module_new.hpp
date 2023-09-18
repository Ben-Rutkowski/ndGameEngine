#ifndef ND_MODULE_NEW_HPP
#define ND_MODULE_NEW_HPP

#include "event_manager_new.hpp"

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

class ndModuleNew {
protected:
    Module module_name;
    DimensionCacheTEMPORARY dcache;
    StateCacheTEMPORARY     scache;

public:
    ndModuleNew(Module mod_in, int state_len)
        :module_name{ mod_in },
        scache(state_len) {}

// --- Manager Init ---
public:
    virtual void setManagerPtr(EventManagerNew* ptr) = 0;

protected:
    virtual void setCallbacks() = 0;

// --- Requests ---
public:
    virtual bool requestBool(Request request) { return false; }

// --- Event Interface ---
public:
    virtual void runEvent(Event* event) = 0;
};

template<typename T>
struct onEventFunc {
    EventCallTypeNew<T> func;
    bool is_null = true;
};

template<typename T>
struct EventInterfaceStruct {
typedef EnumMap<Data, onEventFunc<T>, EVENT_DATA_LEN> FunctionArray;

    FunctionArray    on_event;
    EventManagerNew* manager;
};


template<typename MOD>
class ndModuleInstance : public ndModuleNew {

protected:
    EventInterfaceStruct<MOD> event_interface_new;

public:
    ndModuleInstance(Module mod_in, int state_len)
        :ndModuleNew(mod_in, state_len) {}

    void setManagerPtr(EventManager* ptr) {
        event_interface.manager = ptr;
    }

    void setEvent(Data data, EventCallTypeNew<MOD> func) {
        event_interface.on_event.addEntry(
            data, onEventFunc<MOD>{ func, false };
        )
    }

    void runEvent(Event* event) {
        Data data = event->getData();
        onEventFunc<MOD> on_event = event_interface.on_event.get(data);
        if (!on_event.is_null) {
            on_event.func(this, event);
        } else {
            std::cout << "Event Callback Not Set" << std::endl;
        }
    }

    // ndModuleInstance(Module mod_in, int state_len)
    //     :ndModuleNew(mod_in, state_len) {}

    // void linkManager(void* ptr, EventCallTypeStatic func) {
    //     event_interface.linkManager(ptr, func);
    // }

};

#endif