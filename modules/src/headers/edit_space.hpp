#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "event_manager.hpp"

class EditSpace {
private: static Module module_name;
// --- Attributes ---
private:
    EventInterface event_interface;

public:
    EditSpace();
    void setManagerPtr(EventManager* ptr);

// --- Event Interface ---
public:
    void runEventEditSpace(Event* event);

private:
    void setCallbacks();

// --- On Events ---
private:
    void onBeginLoop(Event* event);

};

#endif