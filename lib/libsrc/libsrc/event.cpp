#include <iostream>
#include "event.hpp"

// === Structs ===
EventCode::EventCode()
    :type{ EventType::null },
    module{ Module::null },
    data{ Data::null } {}

EventCode::EventCode(EventType type_in, Module module_in, Data data_in)
    :type{ type_in },
    module{ module_in },
    data{ data_in } {}

// === Constructors and Initialization ===
Event::Event() {}

Event::Event(EventType type, Module module_name, Data data)
    :code{ EventCode(type, module_name, data) } {}

Event::Event(Module module_name, Data data)
    :code{ EventCode(EventType::ACTION, module_name, data) } {}

// === Gets and Sets ===
EventType Event::getType()   { return code.type; }
Module    Event::getModule() { return code.module; }
Data      Event::getData()   { return code.data; }
bool      Event::operator!() { return code.type == EventType::null; }

// === Virtual ===
int   Event::getInt(int index)   { return 0; }
float Event::getFloat(int index) { return 0.0f; }
vec2  Event::getVec2()           { return vec2(0.0f); }
vec4  Event::getVec4()           { return vec4(0.0f); }
vec2i Event::getVec2i()          { return vec2i(0); }
vec4i Event::getVec4i()          { return vec4i(0); }

// === Debugging ===
void Event::print() {
    std::cout << EnumMapIndex::moduleName(code.module);
    std::cout << " event: ";
    std::cout << EnumMapIndex::dataName(code.data) << std::endl;
}

void Event::print(Module module_name) {
    std::cout << EnumMapIndex::moduleName(code.module);
    std::cout << " event: ";
    std::cout << EnumMapIndex::dataName(code.data);
    std::cout << " in " << EnumMapIndex::moduleName(module_name) << std::endl;
}